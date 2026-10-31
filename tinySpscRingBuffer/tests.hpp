#pragma once
#include "main.h"

// ============================================================
// PartII: Test
// ============================================================
// 不用 assert，避免 -DNDEBUG 导致测试表达式不执行。
#define CHECK(expression)                                              \
    do {                                                               \
        if (!(expression)) {                                           \
            std::cerr << "CHECK failed: " << #expression                \
                      << " at line " << __LINE__ << '\n';              \
            std::abort();                                              \
        }                                                              \
    } while (false)

void test_empty_full_fifo() {
    SpscRingBuffer<std::uint64_t, 4> ring;

    CHECK(ring.capacity() == 4);

    std::uint64_t out = 999;
    CHECK(!ring.try_pop(out));
    CHECK(out == 999);

    for (std::uint64_t i = 0; i < 4; ++i) {
        CHECK(ring.try_push(i));
    }

    CHECK(!ring.try_push(100));

    for (std::uint64_t i = 0; i < 4; ++i) {
        CHECK(ring.try_pop(out));
        CHECK(out == i);
    }

    out = 999;
    CHECK(!ring.try_pop(out));
    CHECK(out == 999);
}

void test_capacity_one() {
    SpscRingBuffer<std::uint64_t, 1> ring;

    for (std::uint64_t i = 0; i < 1000; ++i) {
        CHECK(ring.try_push(i));
        CHECK(!ring.try_push(i + 1));

        std::uint64_t out = 0;
        CHECK(ring.try_pop(out));
        CHECK(out == i);
        CHECK(!ring.try_pop(out));
    }
}

void test_slot_wraparound() {
    SpscRingBuffer<std::uint64_t, 4> ring;

    CHECK(ring.try_push(0));
    CHECK(ring.try_push(1));
    CHECK(ring.try_push(2));

    std::uint64_t out = 0;

    CHECK(ring.try_pop(out));
    CHECK(out == 0);
    CHECK(ring.try_pop(out));
    CHECK(out == 1);

    // 写入位置跨过数组尾部，同时保留未消费的 2。
    CHECK(ring.try_push(3));
    CHECK(ring.try_push(4));
    CHECK(ring.try_push(5));
    CHECK(!ring.try_push(6));

    for (std::uint64_t expected = 2; expected <= 5; ++expected) {
        CHECK(ring.try_pop(out));
        CHECK(out == expected);
    }

    CHECK(!ring.try_pop(out));
}

void test_counter_overflow() {
    constexpr auto max =
        std::numeric_limits<std::uint64_t>::max();

    SpscRingBuffer<std::uint64_t, 4> ring(max - 2);

    // 第一轮填满就会让 write_pos 跨过 UINT64_MAX。
    for (std::uint64_t round = 0; round < 100; ++round) {
        for (std::uint64_t i = 0; i < 4; ++i) {
            CHECK(ring.try_push(round * 4 + i));
        }

        CHECK(!ring.try_push(999));

        for (std::uint64_t i = 0; i < 4; ++i) {
            std::uint64_t out = 0;
            CHECK(ring.try_pop(out));
            CHECK(out == round * 4 + i);
        }

        std::uint64_t out = 123;
        CHECK(!ring.try_pop(out));
        CHECK(out == 123);
    }
}

void test_random_against_deque() {
    constexpr std::size_t capacity = 16;

    SpscRingBuffer<std::uint64_t, capacity> ring;
    std::deque<std::uint64_t> reference;
    std::mt19937_64 rng(12345);

    for (std::size_t i = 0; i < 200'000; ++i) {
        if ((rng() & 1) != 0) {
            const std::uint64_t value = rng();
            const bool expected = reference.size() < capacity;

            CHECK(ring.try_push(value) == expected);

            if (expected) {
                reference.push_back(value);
            }
        } else {
            std::uint64_t out = 777;
            const bool expected = !reference.empty();

            CHECK(ring.try_pop(out) == expected);

            if (expected) {
                CHECK(out == reference.front());
                reference.pop_front();
            } else {
                CHECK(out == 777);
            }
        }
    }

    while (!reference.empty()) {
        std::uint64_t out = 0;
        CHECK(ring.try_pop(out));
        CHECK(out == reference.front());
        reference.pop_front();
    }

    std::uint64_t out = 0;
    CHECK(!ring.try_pop(out));
}

struct Command {
    std::uint64_t sequence = 0;
    std::uint64_t payload = 0;
};

constexpr std::uint64_t make_payload(std::uint64_t sequence) noexcept {
    return sequence ^ 0xA5A5A5A55A5A5A5AULL;
}

void test_spsc_threads() {
    constexpr std::uint64_t count = 1'000'000;

    // 同时覆盖并发运行时的计数器溢出。
    SpscRingBuffer<Command, 1024> ring(
        std::numeric_limits<std::uint64_t>::max() - 100);

    std::atomic<bool> start{false};

    std::thread producer([&] {
        while (!start.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        for (std::uint64_t i = 0; i < count; ++i) {
            const Command command{i, make_payload(i)};

            while (!ring.try_push(command)) {
                std::this_thread::yield();
            }
        }
    });

    std::thread consumer([&] {
        while (!start.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }

        for (std::uint64_t expected = 0; expected < count; ++expected) {
            Command command;

            while (!ring.try_pop(command)) {
                std::this_thread::yield();
            }

            CHECK(command.sequence == expected);
            CHECK(command.payload == make_payload(expected));
        }
    });

    start.store(true, std::memory_order_release);

    producer.join();
    consumer.join();

    // 两个工作线程已经退出，可以安全地在这里检查。
    Command command{123, 456};
    CHECK(!ring.try_pop(command));
    CHECK(command.sequence == 123);
    CHECK(command.payload == 456);
}



