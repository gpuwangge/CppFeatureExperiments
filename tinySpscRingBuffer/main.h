#pragma once
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <random>
#include <thread>
#include <type_traits>

/*
固定大小元素队列
struct Command {
    uint64_t sequence;
    uint32_t opcode;
    uint32_t argument;
};
创建时一次性分配所有槽位。
push/pop 内不发生动态分配。
容量固定，不自动扩容。
容量要求为 2 的幂，方便用 index & (N - 1) 映射物理槽位；
FIFO 和回绕
- 按写入顺序读出。
- 写到数组尾部后回到开头。
- 重复绕环仍然正确。
- 区分“逻辑位置”和“数组位置”。

write_pos：下一次写入的逻辑位置
read_pos ：下一次读取的逻辑位置

used = write_pos - read_pos
slot = pos & (capacity - 1)

empty：used == 0
full ：used == capacity

非阻塞接口和明确的失败语义
bool try_push(const T& item); // 满时 false，不改变队列
bool try_pop(T& out);        // 空时 false，不修改 out
size_t capacity() const noexcept;

约定：
- 满时拒绝写入，不覆盖旧数据。
- 空时立即失败，不在内部等待。
- 不使用异常表达“满/空”。
- 成功操作严格 FIFO。

明确线程角色
- 只有 producer 能修改 write_pos。
- 只有 consumer 能修改 read_pos。
- producer 和 consumer 可以同时执行。
- 不允许两个线程同时调用 try_push，也不允许两个线程同时调用 try_pop。
这正是 SPSC 的边界；要支持多个 producer 或 consumer，需要额外串行化或另一套算法，不能直接沿用同样的假设。

*/

// 固定容量，单生产者、单消费者。
// 同一时刻只允许一个线程调用 try_push，
// 同一时刻只允许一个线程调用 try_pop。
template <typename T, std::size_t Capacity>
class SpscRingBuffer {
    static_assert(Capacity > 0, "Capacity must be positive");
    static_assert((Capacity & (Capacity - 1)) == 0,
                  "Capacity must be a power of two");

    static_assert(
        Capacity <= std::numeric_limits<std::uint64_t>::max() / 2,
        "Capacity is too large");

    static_assert(std::is_trivially_copyable_v<T>,
                  "T must be trivially copyable");
    static_assert(std::is_default_constructible_v<T>,
                  "T must be default constructible");
    static_assert(std::is_nothrow_copy_assignable_v<T>,
                  "T must support noexcept copy assignment");

    using Counter = std::uint64_t;

private:
    static constexpr std::size_t slot_index(Counter position) noexcept {
        return static_cast<std::size_t>(
            position & static_cast<Counter>(Capacity - 1));
    }

    std::array<T, Capacity> m_slots{};
    std::atomic<Counter> m_write_pos;
    std::atomic<Counter> m_read_pos;

public:
    // initial_position 只用于方便测试计数器溢出。
    // 无论初始位置是多少，新建队列都是空的。
    explicit SpscRingBuffer(Counter initial_position = 0) noexcept
        : m_write_pos(initial_position),
          m_read_pos(initial_position) {}

    SpscRingBuffer(const SpscRingBuffer&) = delete;
    SpscRingBuffer& operator=(const SpscRingBuffer&) = delete;

    static constexpr std::size_t capacity() noexcept {
        return Capacity;
    }

    // 只允许 producer 调用。
    // 满时返回 false，不修改队列。
    bool try_push(const T& item) noexcept {
        const Counter write =
            m_write_pos.load(std::memory_order_relaxed);

        const Counter read =
            m_read_pos.load(std::memory_order_acquire);

        if (write - read == static_cast<Counter>(Capacity)) {
            return false;
        }

        m_slots[slot_index(write)] = item;

        // 发布已经写完的槽位。
        m_write_pos.store(write + Counter{1},
                         std::memory_order_release);
        return true;
    }

    // 只允许 consumer 调用。
    // 空时返回 false，不修改 out。
    bool try_pop(T& out) noexcept {
        const Counter read =
            m_read_pos.load(std::memory_order_relaxed);

        const Counter write =
            m_write_pos.load(std::memory_order_acquire);

        if (read == write) {
            return false;
        }

        out = m_slots[slot_index(read)];

        // 读完后才允许 producer 复用槽位。
        m_read_pos.store(read + Counter{1},
                        std::memory_order_release);
        return true;
    }
};

// ============================================================
// PartI: SPSC Ring Buffer
// ============================================================
