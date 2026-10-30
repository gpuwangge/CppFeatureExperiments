#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <vector>
#include <optional>

/*
TinyPeekQueue
A thread-safe bounded queue with try-push, try-pop, and peek for GPU modeling. No Close()

TinyPeekQueue<T> 是一个容量固定、线程安全的队列，支持 MPMC（多生产者、多消费者），用于在线程之间传递数据。

- 基本功能：
push() 尝试入队，成功返回 true；
pop() 尝试取出队首元素，成功返回数据；
peek() 查看当前元素数量以及是否可以入队、出队，不修改队列。队列满时拒绝入队，队列空时无法出队。

- 实现原理：
使用 std::queue<T> 存储数据，通过同一个互斥锁保护队列的检查和修改。
push/pop/peek 都采用尝试加锁方式，锁被占用时立即返回失败，不等待锁、数据或空位；
但这并不是无锁队列，也不保证操作耗时固定。

- 使用注意：
peek() 仅提供状态快照，不保证下一次操作成功，应以 push/pop 的返回值为准。
该类不提供 close()，生产结束和消费者退出由外部逻辑控制。

多线程在这个模型里的意义
多线程的主要价值是加速模拟器执行，而不是用 CPU 线程的并发来代替 GPU 硬件的并发。
单线程也能模拟 GPU 的并行行为：例如，每个模拟周期依次调用各个模块
只要正确隔离当前状态和下一状态，软件执行顺序就不必等于硬件执行顺序。GPU 上同周期发生的行为，可以在宿主 CPU 上依次计算。
这个类可以保障宿主线程之间安全传递数据，但所有队列访问仍通过同一把锁串行执行。因此：
- 如果线程取出任务后，会做大量独立计算，队列可以作为有效的任务分发工具。
- 如果线程的大部分工作就是频繁 push/pop，这把锁可能成为竞争点。
- 如果用于周期模型，push/pop 抢锁失败只能算宿主执行层的问题，不能直接变成模拟 GPU 的 stall。
比喻：
以把 TinyPeekQueue 想成一个“取任务的柜台”，多个线程就是多个工人。
这个柜台只有一把钥匙，也就是 互斥锁。放任务、取任务、查看柜台，都必须先拿到钥匙。你的实现是不排队等钥匙：拿不到就先返回失败。
1. 任务很重，多线程就可能有用 (加快仿真速度)
2. 任务很轻，大家就可能都在抢柜台
3. 抢不到钥匙，不代表模拟 GPU 的队列满了:当前 push() 返回 false，有两种可能：
情况 A：拿到钥匙，发现柜台已经放满任务。
情况 B：没拿到钥匙，根本还没检查柜台有没有空间。情况 B 中，队列可能还有很多空位，只是另一个 CPU 线程正在操作它。
一句话：柜台决定工人怎么交换任务；工人抢柜台的快慢，不应该决定你模拟出来的 GPU 有多快。

这个队列模型适合用于 functional model 的数据传递，不能直接作为 performance model 的硬件时序模型。
模拟 GPU FIFO 的延迟、吞吐、仲裁、stall	不能直接使用，需要额外建模。
functional model 也要注意：多生产者的入队顺序取决于实际获得锁的顺序。如果功能正确性要求某些请求严格有序，仍然需要由外部协议保证；线程安全本身不保证符合目标 GPU 的顺序语义。

是什么机制保证了多线程的安全
靠的是同一把 互斥锁 m_mutex：线程必须先拿到锁，才能检查或修改队列；拿不到就返回失败。因此，同一时刻只有一个线程操作队列，避免多个线程同时读写造成数据竞争。

怎么接入 cycle-based model
一种清晰的设计是分离“计算”和“提交”：
for (uint64_t cycle = 0; cycle < maxCycles; ++cycle) {
    // 根据当前状态，计算本周期请求与下一状态。
    producer.evaluate();
    consumer.evaluate();

    // 根据端口限制和仲裁规则，统一提交变化。
    fifo.commit();
    producer.commit();
    consumer.commit();
}

几个概念的关系
这些分类不是互斥的：
- Transaction-based：模块交换什么。
- Event-based / cycle-based：模型如何推进执行。
- Functional / performance：关注功能，还是还要预测性能。
因此，可以构建“按周期推进、以事务通信的 GPU 模型”；但“每周期执行一次”本身并不保证 cycle-accurate。对你当前这个类，准确的定位仍是线程通信工具；要模拟 GPU 内部队列，需要补充显式的周期语义。

*/

// ============================================================
// PartI: TinyPeekQueue
// ============================================================
template <typename T>
class TinyPeekQueue {
public:
    struct State {
        bool canPush;
        bool canPop;
        std::size_t size;
    };

private:
    const std::size_t m_capacity;

    std::queue<T> m_queue;
    mutable std::mutex m_mutex;

    //用 T 和 U 不是为了存两种数据，而是让同一份内部实现兼容“复制入队”和“移动入队”。
    template <typename U>
    bool pushImpl(U&& value) {
        //这里用 std::unique_lock，主要因为它同时提供了 尝试加锁、检查结果和自动解锁
        //unique_lock 离开作用域时自动解锁 (也可以提前手动解锁:lock.unlock(); // 立即解锁)
        //管理 std::mutex 加锁和解锁的类型
        //std::try_to_lock	指定“尝试获取锁，不等待”的策略
        std::unique_lock<std::mutex> lock(
            m_mutex, std::try_to_lock
        );

        // 锁被占用：本次入队失败。
        if (!lock.owns_lock()) {
            return false;
        }

        // 队列已满：本次入队失败。
        if (m_queue.size() >= m_capacity) {
            return false;
        }

        //函数自己声明、并根据参数推导的 U，才让 U&& 成为转发引用。
        m_queue.push(std::forward<U>(value)); 
        return true;
    }

public:
    explicit TinyPeekQueue(std::size_t capacity): m_capacity(capacity) {
        if (m_capacity == 0) {
            throw std::invalid_argument(
                "capacity must be greater than zero"
            );
        }
    }

    TinyPeekQueue(const TinyPeekQueue&) = delete;
    TinyPeekQueue& operator=(const TinyPeekQueue&) = delete;

    //两个 push() 是函数重载，分别支持“复制入队”和“移动入队”。
    //更重要的是，这样可以保证 失败不移动：队列满或拿不到锁、返回 false 时，调用者仍然保留原来的数据。
    // 成功时复制入队。
    bool push(const T& value) {
        return pushImpl(value);
    }
    // 成功时移动入队。
    // 返回 false 时，不会移动 value。
    bool push(T&& value) {
        return pushImpl(std::move(value));
    }

    // 队列为空或锁被占用时，返回 nullopt。
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(
            m_mutex, std::try_to_lock
        );

        if (!lock.owns_lock() || m_queue.empty()) {
            return std::nullopt;
        }

        std::optional<T> value(
            std::in_place, std::move(m_queue.front())
        );

        m_queue.pop();
        return value;
    }

    // 返回当前状态快照，不取出数据、不预留空位。
    // 锁被占用时，返回 nullopt。
    std::optional<State> peek() const {
        std::unique_lock<std::mutex> lock(
            m_mutex, std::try_to_lock
        );

        if (!lock.owns_lock()) {
            return std::nullopt;
        }

        return State{
            m_queue.size() < m_capacity,
            !m_queue.empty(),
            m_queue.size()
        };
    }
};