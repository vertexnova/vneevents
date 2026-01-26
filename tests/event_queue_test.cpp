/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include "vertexnova/events/events.h"
#include <thread>
#include <vector>

namespace vne::events {

// ============================================================================
// EventQueue Tests
// ============================================================================

TEST(EventQueueTest, EmptyQueue) {
    EventQueue queue;

    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(EventQueueTest, PushAndPop) {
    EventQueue queue;

    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 1u);

    auto event = queue.pop();
    EXPECT_NE(event, nullptr);
    EXPECT_EQ(event->type(), EventType::eKeyPressed);
    EXPECT_TRUE(queue.empty());
}

TEST(EventQueueTest, PopEmptyQueue) {
    EventQueue queue;

    auto event = queue.pop();
    EXPECT_EQ(event, nullptr);
}

TEST(EventQueueTest, MultiplePushPop) {
    EventQueue queue;

    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    queue.push(std::make_unique<KeyReleasedEvent>(KeyCode::eB));
    queue.push(std::make_unique<MouseMovedEvent>(10.0, 20.0));

    EXPECT_EQ(queue.size(), 3u);

    auto e1 = queue.pop();
    EXPECT_EQ(e1->type(), EventType::eKeyPressed);

    auto e2 = queue.pop();
    EXPECT_EQ(e2->type(), EventType::eKeyReleased);

    auto e3 = queue.pop();
    EXPECT_EQ(e3->type(), EventType::eMouseMoved);

    EXPECT_TRUE(queue.empty());
}

TEST(EventQueueTest, FIFOOrder) {
    EventQueue queue;

    for (int i = 0; i < 10; ++i) {
        queue.push(std::make_unique<KeyRepeatEvent>(KeyCode::eA, static_cast<uint32_t>(i)));
    }

    for (int i = 0; i < 10; ++i) {
        auto event = queue.pop();
        ASSERT_NE(event, nullptr);
        auto* repeat_event = dynamic_cast<KeyRepeatEvent*>(event.get());
        ASSERT_NE(repeat_event, nullptr);
        EXPECT_EQ(repeat_event->repeatCount(), static_cast<uint32_t>(i));
    }
}

TEST(EventQueueTest, Clear) {
    EventQueue queue;

    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eB));
    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eC));

    EXPECT_EQ(queue.size(), 3u);

    queue.clear();

    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0u);
}

TEST(EventQueueTest, ThreadSafety) {
    EventQueue queue;
    constexpr int kNumThreads = 4;
    constexpr int kEventsPerThread = 100;

    std::vector<std::thread> producers;
    for (int t = 0; t < kNumThreads; ++t) {
        producers.emplace_back([&queue, t]() {
            for (int i = 0; i < kEventsPerThread; ++i) {
                queue.push(std::make_unique<KeyRepeatEvent>(
                    KeyCode::eA, static_cast<uint32_t>(t * kEventsPerThread + i)));
            }
        });
    }

    for (auto& thread : producers) {
        thread.join();
    }

    EXPECT_EQ(queue.size(), static_cast<size_t>(kNumThreads * kEventsPerThread));

    // Consume all events
    int count = 0;
    while (!queue.empty()) {
        auto event = queue.pop();
        if (event) {
            ++count;
        }
    }

    EXPECT_EQ(count, kNumThreads * kEventsPerThread);
}

TEST(EventQueueTest, ConcurrentPushPop) {
    EventQueue queue;
    std::atomic<int> produced{0};
    std::atomic<int> consumed{0};

    constexpr int kProducerCount = 2;
    constexpr int kConsumerCount = 2;
    constexpr int kEventsPerProducer = 100;

    std::vector<std::thread> threads;

    // Producers
    for (int i = 0; i < kProducerCount; ++i) {
        threads.emplace_back([&queue, &produced]() {
            for (int j = 0; j < kEventsPerProducer; ++j) {
                queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
                ++produced;
            }
        });
    }

    // Consumers
    for (int i = 0; i < kConsumerCount; ++i) {
        threads.emplace_back([&queue, &consumed, &produced]() {
            while (consumed < kProducerCount * kEventsPerProducer) {
                auto event = queue.pop();
                if (event) {
                    ++consumed;
                } else if (produced >= kProducerCount * kEventsPerProducer) {
                    // All produced, but queue empty - check again
                    std::this_thread::yield();
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(produced.load(), kProducerCount * kEventsPerProducer);
    EXPECT_EQ(consumed.load(), kProducerCount * kEventsPerProducer);
}

}  // namespace vne::events
