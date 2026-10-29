#include <iostream>

#include <mutex>
#include <condition_variable>
#include <thread>

#include <queue>
#include <vector>
#include <optional>

/*
TinyThreadQueue 是一个基于 C++17 的轻量级有界阻塞队列，用于多生产者、多消费者（MPMC）场景。
- 线程安全：使用同一把互斥锁保护队列和关闭状态，支持多个线程并发调用 push()、pop() 和 close()。
- 背压（Backpressure）：队列满时，push() 阻塞等待；消费者取出数据后，唤醒等待中的生产者继续提交，避免队列无限增长。
- 阻塞消费：队列为空时，pop() 等待新数据，不进行忙等。
- 优雅关闭：close() 停止接收新任务，并唤醒所有等待线程。已有任务仍可继续取出；队列关闭且为空后，pop() 返回 std::nullopt，消费者可据此退出。允许重复关闭。
- 测试覆盖：包含 FIFO、多生产者多消费者、任务无丢失与无重复，以及关闭和排空行为的测试。
- 队列不负责创建或回收线程；调用方需要在销毁队列前，确保所有访问它的线程已经退出。

真正保护线程安全的是哪里？在队列实现内部：lock
*/


// ============================================================
// PartI: TinyThreadQueue
// ============================================================
template <typename T>
class TinyThreadQueue {
private:
    const std::size_t m_capacity;

    std::queue<T> m_queue;
    //close：不再接受新任务
    //close() 不是清空队列，也不是立即停止消费者，更不会等待所有任务处理完成。
    bool bClosed = false; 

    //drain：把已有任务取完,队列为空了
    //但是队列为空 ≠ 所有任务处理完成
    //任务可能已经被 pop() 取走，但消费者还在 process()。
    //在当前框架中，等所有消费者 join() 返回，才表示这些消费者的处理逻辑都已经结束。

    std::mutex m_mutex; //保护共享数据，避免多个线程同时操作造成数据竞争。
    std::condition_variable m_notEmpty; //让线程等待共享状态发生变化
    std::condition_variable m_notFull;  //让线程等待共享状态发生变化
    //std::condition_variable 变量的方法有：
    //.wait(lock, []): 条件不满足就等待，满足才继续
    //.notify_one(): 唤醒一个正在等待 cv 的线程
    //.notify_all(): 唤醒所有正在等待 cv 的线程

public:
    explicit TinyThreadQueue(std::size_t capacity): m_capacity(capacity){
        if (m_capacity == 0) throw std::invalid_argument("capacity must be greater than zero");
    }
    TinyThreadQueue(const TinyThreadQueue&) = delete;
    TinyThreadQueue& operator=(const TinyThreadQueue&) = delete;

    // 队列满时等待；关闭后拒绝新数据。
    bool push(T value){
        //普通代码中可以直接调用 m_mutex.lock() / unlock()；但你这里要调用 std::condition_variable::wait()，
        //它的接口要求传入 unique_lock，不能直接传入 mutex。
        std::unique_lock<std::mutex> lock(m_mutex);

        //先要确认队列有位置才能push
        //等待“队列已经关闭”或者“队列还有空位”。
        //只要其中一个条件成立，wait() 就返回，继续执行后面的代码。
        //这里的做法是为了处理backpressure：下游慢，让上游等待
        m_notFull.wait(lock, [this] {
            //这里有两个不同的唤醒原因：
            //消费者取走数据，队列有空间	入队成功，这是背压解除
            //调用 close()	返回 false，这是退出，不是恢复生产
            return bClosed || m_queue.size() < m_capacity;
        });

        if (bClosed) return false;

        m_queue.push(std::move(value));

        lock.unlock();
        m_notEmpty.notify_one();
        return true;
    }

    // 队列空时等待。
    // 关闭后继续取出剩余数据；关闭且为空时返回 nullopt。
    // std::optional<T> 用来表示：可能有值，也可能没有值。
    std::optional<T> pop(){
        std::unique_lock<std::mutex> lock(m_mutex);

        //先要确认队列不为空才能pop
        //等待“队列已经关闭”或者“队列里有数据”。任意一个条件成立，wait() 就返回。
        m_notEmpty.wait(lock, [this] {
            return bClosed || !m_queue.empty();
        });

        if (m_queue.empty())  return std::nullopt;
        
        std::optional<T> value(std::move(m_queue.front()));
        m_queue.pop();

        lock.unlock();
        m_notFull.notify_one();
        return value;
    }

    // 允许重复调用，不清空已有数据。
    void close(){
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            bClosed = true;
        }

        m_notEmpty.notify_all(); // 唤醒等待数据的消费者：没有新任务了，别一直等
        m_notFull.notify_all(); // 唤醒等待空间的生产者：不再接收任务了，别一直等
    }
};
// ============================================================
// PartII: Test
// ============================================================
// Help Function: 不使用 assert，避免 Release 构建禁用断言后跳过检查。
void check(bool condition, const char* message){
    if (!condition) throw std::runtime_error(message);
}

// ============================================================
// Test 1: 单生产者、单消费者，检查 FIFO
// ============================================================
void test_fifo(){
    constexpr int task_count = 1000;

    TinyThreadQueue<int> queue(4);
    std::vector<int> received;
    received.reserve(task_count);

    std::thread consumer([&] {
        while (auto value = queue.pop()) {
            received.push_back(*value);
        }
    });

    std::thread producer([&] {
        for (int i = 0; i < task_count; ++i) {
            if (!queue.push(i)) {
                return;
            }
        }
    });

    producer.join();
    queue.close();
    consumer.join();

    check(received.size() == task_count, "FIFO: incorrect task count");

    for (int i = 0; i < task_count; ++i) {
        check(received[i] == i, "FIFO: incorrect order");
    }

    std::cout << "[PASS] single producer / single consumer FIFO\n";
}

// ============================================================
// Test 2: 多生产者、多消费者，检查无丢失、无重复
// ============================================================
void test_mpmc(std::size_t capacity){
    constexpr int producer_count = 4;
    constexpr int consumer_count = 3;
    constexpr int tasks_per_producer = 1000;
    constexpr int total_tasks = producer_count * tasks_per_producer;

    TinyThreadQueue<int> queue(capacity);

    // 每个消费者独占自己的结果数组，不需要额外加锁。
    std::vector<std::vector<int>> results(consumer_count);

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int c = 0; c < consumer_count; ++c) {
        consumers.emplace_back([&, c] {
            while (auto task = queue.pop()) {
                // 模拟处理任务；此时已经不持有队列锁。
                results[c].push_back(*task);
            }
        });
    }

    for (int p = 0; p < producer_count; ++p) {
        producers.emplace_back([&, p] {
            for (int i = 0; i < tasks_per_producer; ++i) {
                const int task_id = p * tasks_per_producer + i;

                if (!queue.push(task_id)) {
                    return;
                }
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    queue.close();

    for (auto& consumer : consumers) {
        consumer.join();
    }

    // 所有线程退出后，再统一检查结果。
    std::vector<int> seen(total_tasks, 0);
    std::size_t received_count = 0;

    for (const auto& consumer_results : results) {
        for (int task_id : consumer_results) {
            check(task_id >= 0 && task_id < total_tasks,
                  "MPMC: invalid task ID");

            ++seen[task_id];
            ++received_count;
        }
    }

    check(received_count == total_tasks, "MPMC: incorrect task count");

    for (int count : seen) {
        check(count == 1, "MPMC: missing or duplicated task");
    }

    std::cout << "[PASS] multi producer / multi consumer"
              << " (capacity=" << capacity << ")\n";
}

// ============================================================
// Test 3: 关闭后排空、拒绝 push、重复关闭
// ============================================================
void test_close_and_drain(){
    TinyThreadQueue<int> queue(2);

    check(queue.push(10), "Initial push failed");
    check(queue.push(20), "Initial push failed");

    queue.close();
    queue.close();

    check(!queue.push(30), "Closed queue accepted new data");

    check(queue.pop() == std::optional<int>{10}, "Drain: expected 10");
    check(queue.pop() == std::optional<int>{20}, "Drain: expected 20");
    check(!queue.pop().has_value(), "Closed empty queue should return nullopt");

    std::cout << "[PASS] close and drain\n";
}

// ============================================================
// Test 4: 空队列关闭，让消费者退出
// ============================================================
void test_close_empty_queue(){
    TinyThreadQueue<int> queue(1);
    bool received_nothing = false;

    std::thread consumer([&] {
        received_nothing = !queue.pop().has_value();
    });

    queue.close();
    consumer.join();

    check(received_nothing, "Consumer did not exit correctly");

    std::cout << "[PASS] close empty queue\n";
}

// ============================================================
// Test 5: 满队列关闭，让生产者退出
// ============================================================
void test_close_full_queue(){
    TinyThreadQueue<int> queue(1);
    check(queue.push(10), "Initial push failed");

    bool push_succeeded = true;

    std::thread producer([&] {
        push_succeeded = queue.push(20);
    });

    queue.close();
    producer.join();

    check(!push_succeeded, "Producer should reject push after close");
    check(queue.pop() == std::optional<int>{10}, "Existing data was lost");
    check(!queue.pop().has_value(), "Queue should be drained");

    std::cout << "[PASS] close full queue\n";
}

// ============================================================
// PartIII: Main
// ============================================================

int main(){
	std::cout<<"===Thread Queue Test begin==="<<std::endl;

	try {
        test_fifo();

        test_mpmc(1);
        test_mpmc(16);

        test_close_and_drain();
        test_close_empty_queue();
        test_close_full_queue();

        std::cout << "\nAll tests passed.\n";
        //return 0;
    }
    catch (const std::exception& error) {
        std::cerr << "[FAIL] " << error.what() << '\n';
        return 1;
    }
	
	std::cout<<"===Thread Queue Test end==="<<std::endl;
	return 0;
}

/* 运行结果
===Thread Queue Test begin===
[PASS] single producer / single consumer FIFO
[PASS] multi producer / multi consumer (capacity=1)
[PASS] multi producer / multi consumer (capacity=16)
[PASS] close and drain
[PASS] close empty queue
[PASS] close full queue

All tests passed.
===Thread Queue Test end===
*/

/* 结果说明

*/

