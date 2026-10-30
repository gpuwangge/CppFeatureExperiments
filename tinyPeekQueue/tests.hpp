#pragma once
#include "main.h"
#include <atomic>
#include <cassert>
#include <memory>

// ============================================================
// PartII: Test
// ============================================================
// 1. 空队列状态。
void testEmptyQueue() {
    TinyPeekQueue<int> queue(2);

    auto state = queue.peek();

    assert(state.has_value());
    assert(state->size == 0);
    assert(state->canPush);
    assert(!state->canPop);

    auto value = queue.pop();
    assert(!value.has_value());
}

// 2. 容量限制、FIFO、取出后重新入队。
void testCapacityAndFIFO() {
    TinyPeekQueue<int> queue(2);

    bool first = queue.push(10);
    bool second = queue.push(20);
    bool overflow = queue.push(30);

    assert(first);
    assert(second);
    assert(!overflow);

    auto fullState = queue.peek();

    assert(fullState.has_value());
    assert(fullState->size == 2);
    assert(!fullState->canPush);
    assert(fullState->canPop);

    auto a = queue.pop();

    assert(a.has_value());
    assert(*a == 10);

    auto availableState = queue.peek();

    assert(availableState.has_value());
    assert(availableState->size == 1);
    assert(availableState->canPush);
    assert(availableState->canPop);

    bool pushedAgain = queue.push(30);
    assert(pushedAgain);

    auto b = queue.pop();
    auto c = queue.pop();
    auto empty = queue.pop();

    assert(b.has_value() && *b == 20);
    assert(c.has_value() && *c == 30);
    assert(!empty.has_value());

    // 清空以后，队列可以继续使用。
    bool reused = queue.push(40);
    auto d = queue.pop();

    assert(reused);
    assert(d.has_value() && *d == 40);
}

// 3. peek 不修改队列。
void testPeekDoesNotModifyQueue() {
    TinyPeekQueue<int> queue(2);

    bool pushed = queue.push(42);
    assert(pushed);

    for (int i = 0; i < 3; ++i) {
        auto state = queue.peek();

        assert(state.has_value());
        assert(state->size == 1);
        assert(state->canPush);
        assert(state->canPop);
    }

    auto value = queue.pop();
    auto empty = queue.pop();

    assert(value.has_value() && *value == 42);
    assert(!empty.has_value());
}

// 4. capacity == 0 必须抛出异常。
void testInvalidCapacity() {
    bool caught = false;

    try {
        TinyPeekQueue<int> queue(0);
    } catch (const std::invalid_argument&) {
        caught = true;
    }

    assert(caught);
}

// 5. 支持只能移动的类型。
//    push 返回 false 时，调用者仍保留数据。
void testMoveOnlyType() {
    TinyPeekQueue<std::unique_ptr<int>> queue(1);

    auto first = std::make_unique<int>(10);
    bool firstPushed = queue.push(std::move(first));

    assert(firstPushed);
    assert(!first);

    auto second = std::make_unique<int>(20);
    bool secondPushed = queue.push(std::move(second));

    assert(!secondPushed);
    assert(second);
    assert(*second == 20);

    auto a = queue.pop();

    assert(a.has_value());
    assert(*a);
    assert(**a == 10);

    // 上次失败没有移动 second，这次可以再次尝试。
    bool retried = queue.push(std::move(second));

    assert(retried);
    assert(!second);

    auto b = queue.pop();

    assert(b.has_value());
    assert(*b);
    assert(**b == 20);
}

// 6. MPMC：多个生产者 + 多个消费者。
//    验证每条数据恰好被消费一次。
void testMPMC() {
    constexpr int producerCount = 4;
    constexpr int consumerCount = 4;
    constexpr int itemsPerProducer = 5000;
    constexpr int totalItems =
        producerCount * itemsPerProducer;

    // 小容量，让满队列和锁竞争更容易出现。
    TinyPeekQueue<int> queue(16);

    std::atomic<bool> start{false};
    std::atomic<int> finishedProducers{0};

    // 每个消费者只写自己的 vector，join 后统一验证。
    std::vector<std::vector<int>> received(consumerCount);

    for (auto& values : received) {
        values.reserve(totalItems);
    }

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    producers.reserve(producerCount);
    consumers.reserve(consumerCount);

    for (int c = 0; c < consumerCount; ++c) {
        consumers.emplace_back([&, c] {
            while (!start.load()) {
                std::this_thread::yield();
            }

            for (;;) {
                auto value = queue.pop();

                if (value.has_value()) {
                    received[c].push_back(*value);
                    continue;
                }

                // pop 失败不能直接退出：
                // 可能队列暂时为空，也可能只是没拿到锁。
                //
                // 只有所有生产者已结束，且确认队列为空，
                // 才能确定以后没有数据可取。
                if (finishedProducers.load() == producerCount) {
                    auto state = queue.peek();

                    if (state && state->size == 0) {
                        break;
                    }
                }

                std::this_thread::yield();
            }
        });
    }

    for (int p = 0; p < producerCount; ++p) {
        producers.emplace_back([&, p] {
            while (!start.load()) {
                std::this_thread::yield();
            }

            for (int i = 0; i < itemsPerProducer; ++i) {
                // 每个生产者使用不同的整数区间。
                const int value = p * itemsPerProducer + i;

                while (!queue.push(value)) {
                    std::this_thread::yield();
                }
            }

            // 必须在所有数据成功入队之后再标记完成。
            finishedProducers.fetch_add(1);
        });
    }

    start.store(true);

    for (auto& producer : producers) {
        producer.join();
    }

    for (auto& consumer : consumers) {
        consumer.join();
    }

    // 所有线程结束后，验证取出的数据。
    std::vector<int> seen(totalItems, 0);
    std::size_t consumedCount = 0;

    for (const auto& values : received) {
        consumedCount += values.size();

        for (int value : values) {
            const bool valid =
                value >= 0 && value < totalItems;

            assert(valid);

            // 避免关闭 assert 时出现越界访问。
            if (valid) {
                ++seen[value];
            }
        }
    }

    assert(consumedCount ==
           static_cast<std::size_t>(totalItems));

    for (int count : seen) {
        // 同时检查：没有丢失，也没有重复消费。
        assert(count == 1);
    }

    auto state = queue.peek();

    assert(state.has_value());
    assert(state->size == 0);
    assert(state->canPush);
    assert(!state->canPop);
}

