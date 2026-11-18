#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <limits>
#include <optional>
#include <stdexcept>
#include <iostream>

/*
这段代码的核心考点不是“模拟内存读写”，而是：实现一个具有固定延迟、有限在途容量、FIFO 响应和背压机制的时序模型。

有限在途容量（英文通常称为 Limited In-Flight Capacity 或 Bounded In-Flight Capacity）是指在一个数据传输、任务处理或物流系统中，同时处于“正在传输但尚未确认完成”状态的数据包、请求或任务的最大数量限制。
我们可以用一个简单的生活例子来理解：
• 高速公路类比：一条高速公路同时最多只能容纳 100 辆车在路上行驶。如果有第 101 辆车想上高速，必须等前面有车驶出出口后才能进入。这 100 辆车的上限，就是“有限在途容量”。

什么它叫 memory unit，而不是 FIFO？
在这道题的简化范围内，memory unit 更多是在描述它扮演的角色：
发起方 → 请求 → 模拟内存延迟 → 响应 → 发起方
FIFO 则是实现这个角色的一种内部机制。
真正更详细的内存模型还可能包含 bank 并行、row hit/miss、请求调度、读写切换等行为，这时就远不只是一个 FIFO；但你这题没有必要自行加上这些复杂度。
所以，你把它理解成“带时间约束的 FIFO”，对于我们重建的这个版本是很准确的。真正需要澄清的不是名字，而是：计时从接受开始还是从队首开始、容量何时释放、每周期允许多少请求和响应。

面试主要看三方面：
- 能否准确理解时序与容量语义。
- 能否通过数据结构和不变量证明实现正确。
- 能否处理边界条件、异常安全和模型扩展。

这段代码实际模拟了什么？
可以将其理解为一个简化的 GPU/硬件请求处理单元：
                    到达 ready_cycle
accept(request) ──→ pending_ ───────────→ ready_ ──→ pop_response()
                       tick() 推进时间

                    outstanding = pending + ready
           
各成员的含义：
| 成员        | 含义                   |
| --------- | -------------------- |
| capacity_ | 已接收、但尚未被调用方取走的请求总数上限 |
| latency_  | 从接收到请求到响应可就绪的固定模拟延迟  |
| now_      | 当前模拟周期               |
| pending_  | 已接收、尚未转成响应的请求        |
| ready_    | 已完成、等待调用方取走的响应       |

一个重要区别：它没有真实存储数据，也不读取 address 指向的内容。响应只是回传请求的 id 和 address。
因此，它更接近 memory timing stub，而不是完整的 memory model。

一个时序例子
假设：MemoryUnit unit(2, 3);
| 操作             | now() | pending 数量 | ready 数量 | 结果            |
| -------------- | ----- | ---------- | -------- | ------------- |
| accept(A)      | 0     | 1          | 0        | 成功，A 在周期 3 就绪 |
| accept(B)      | 0     | 2          | 0        | 成功，B 在周期 3 就绪 |
| accept(C)      | 0     | 2          | 0        | 失败，容量已满       |
| tick(2)        | 2     | 2          | 0        | 尚未完成          |
| tick(1)        | 3     | 0          | 2        | A、B 都变成响应     |
| accept(C)      | 3     | 0          | 2        | 仍然失败          |
| pop_response() | 3     | 0          | 1        | 返回 A，释放一个容量   |
| accept(C)      | 3     | 1          | 1        | 成功，C 在周期 6 就绪 |
最后两行体现了最容易被忽略的设计：完成请求并不会释放容量，消费响应才会。

这是固定延迟、允许多个请求并发在途的模型，不是每处理完一个请求才能处理下一个的串行服务器。
| 概念                   | 本代码中的体现              |
| -------------------- | -------------------- |
| Latency              | 每个请求的固定延迟 latency_   |
| Outstanding capacity | 未取走请求的最大数量 capacity_ |
| 接收/完成带宽              | 没有单独建模               |

消费者不取响应
    ↓
ready_ 积压
    ↓
outstanding 达到 capacity
    ↓
accept 返回 false
这就是 背压。

总结
这是一个单线程使用的、固定延迟、有限 outstanding 容量的请求响应时序模型。
请求接收时记录绝对完成周期，tick() 推进模拟时间，并按 FIFO 将到期请求转成响应。
由于时间单调且延迟固定，完成时间天然有序，只检查队首即可。
容量包含 pending 和 ready，因此只有消费响应才会释放容量，形成端到端背压。
实现还处理了时间加法溢出，并通过先生成响应、再删除请求来避免异常时丢失请求；
但 tick() 在内存分配失败时可能部分完成，不具有整体强异常保证。

*/

struct Request {
    std::uint64_t id;
    std::uint64_t address;
};

struct Response {
    std::uint64_t id;
    std::uint64_t address;
};

class MemoryUnit {
public:
    MemoryUnit(std::size_t capacity, std::uint64_t latency)
        : capacity_(capacity), latency_(latency) {
        if (capacity == 0 || latency == 0) {
            throw std::invalid_argument(
                "capacity and latency must be positive");
        }
    }

    bool accept(const Request& request) {
        if (outstanding() >= capacity_) {
            return false;
        }

        const auto ready_cycle = checked_add(now_, latency_);
        pending_.push_back(Pending{request, ready_cycle});
        return true;
    }

    void tick(std::uint64_t cycles = 1) {
        now_ = checked_add(now_, cycles);

        while (!pending_.empty() &&
               pending_.front().ready_cycle <= now_) {
            const auto& request = pending_.front().request;

            ready_.push_back(Response{
                request.id,
                request.address
            });

            pending_.pop_front();
        }
    }

    std::optional<Response> pop_response() {
        if (ready_.empty()) {
            return std::nullopt;
        }

        Response response = ready_.front();
        ready_.pop_front();
        return response;
    }

    std::uint64_t now() const noexcept {
        return now_;
    }

    std::size_t outstanding() const noexcept {
        return pending_.size() + ready_.size();
    }

private:
    struct Pending {
        Request request;
        std::uint64_t ready_cycle;
    };

    static std::uint64_t checked_add(
        std::uint64_t a, std::uint64_t b) {
        if (b > std::numeric_limits<std::uint64_t>::max() - a) {
            throw std::overflow_error("simulation cycle overflow");
        }
        return a + b;
    }

    std::size_t capacity_;
    std::uint64_t latency_;
    std::uint64_t now_ = 0;

    std::deque<Pending> pending_;
    std::deque<Response> ready_;
};


//最小测试  
int main() {
    MemoryUnit memory(/*capacity=*/2, /*latency=*/3);

    // Cycle 0: A accepted, due at cycle 3.
    assert(memory.accept(Request{1, 0x1000}));

    memory.tick();  // Cycle 1

    // Cycle 1: B accepted, due at cycle 4.
    assert(memory.accept(Request{2, 0x2000}));

    // Both slots are occupied.
    assert(!memory.accept(Request{3, 0x3000}));

    memory.tick();  // Cycle 2
    assert(!memory.pop_response().has_value());

    memory.tick();  // Cycle 3: A becomes ready.

    // A still occupies capacity until its response is consumed.
    assert(!memory.accept(Request{3, 0x3000}));

    auto a = memory.pop_response();
    assert(a && a->id == 1 && a->address == 0x1000);

    // The released slot can be reused in the same cycle.
    assert(memory.accept(Request{3, 0x3000}));  // Due at cycle 6.

    memory.tick();  // Cycle 4
    auto b = memory.pop_response();
    assert(b && b->id == 2);

    memory.tick(2);  // Cycle 6
    auto c = memory.pop_response();
    assert(c && c->id == 3);

    assert(memory.outstanding() == 0);
    assert(!memory.pop_response().has_value());

    std::cout << "All tests passed!" << std::endl;
}

