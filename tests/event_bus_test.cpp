/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include <gtest/gtest.h>
#include "vertexnova/events/events.h"
#include <atomic>

namespace vne::events {

// ============================================================================
// Test Listener Implementation
// ============================================================================

class TestListener : public EventListener {
   public:
    void onEvent(const Event& event) override {
        ++event_count;
        last_event_type = event.type();
    }

    std::atomic<int> event_count{0};
    EventType last_event_type{EventType::eInvalid};
};

class CountingListener : public EventListener {
   public:
    explicit CountingListener(std::atomic<int>& counter)
        : counter_(counter) {}

    void onEvent(const Event& /*event*/) override { ++counter_; }

   private:
    std::atomic<int>& counter_;
};

// ============================================================================
// EventDispatcher Tests
// ============================================================================

TEST(EventDispatcherTest, RegisterAndDispatch) {
    EventDispatcher dispatcher;
    auto listener = std::make_shared<TestListener>();

    dispatcher.registerListener(EventType::eKeyPressed, listener);
    EXPECT_EQ(dispatcher.listenerCount(EventType::eKeyPressed), 1u);

    KeyPressedEvent event(KeyCode::eA);
    dispatcher.dispatch(event);

    EXPECT_EQ(listener->event_count.load(), 1);
    EXPECT_EQ(listener->last_event_type, EventType::eKeyPressed);
}

TEST(EventDispatcherTest, UnregisterListener) {
    EventDispatcher dispatcher;
    auto listener = std::make_shared<TestListener>();

    dispatcher.registerListener(EventType::eKeyPressed, listener);
    EXPECT_EQ(dispatcher.listenerCount(EventType::eKeyPressed), 1u);

    dispatcher.unregisterListener(EventType::eKeyPressed, listener.get());
    EXPECT_EQ(dispatcher.listenerCount(EventType::eKeyPressed), 0u);

    KeyPressedEvent event(KeyCode::eA);
    dispatcher.dispatch(event);

    EXPECT_EQ(listener->event_count.load(), 0);
}

TEST(EventDispatcherTest, MultipleListeners) {
    EventDispatcher dispatcher;
    auto listener1 = std::make_shared<TestListener>();
    auto listener2 = std::make_shared<TestListener>();

    dispatcher.registerListener(EventType::eKeyPressed, listener1);
    dispatcher.registerListener(EventType::eKeyPressed, listener2);

    EXPECT_EQ(dispatcher.listenerCount(EventType::eKeyPressed), 2u);

    KeyPressedEvent event(KeyCode::eA);
    dispatcher.dispatch(event);

    EXPECT_EQ(listener1->event_count.load(), 1);
    EXPECT_EQ(listener2->event_count.load(), 1);
}

TEST(EventDispatcherTest, DifferentEventTypes) {
    EventDispatcher dispatcher;
    auto key_listener = std::make_shared<TestListener>();
    auto mouse_listener = std::make_shared<TestListener>();

    dispatcher.registerListener(EventType::eKeyPressed, key_listener);
    dispatcher.registerListener(EventType::eMouseMoved, mouse_listener);

    KeyPressedEvent key_event(KeyCode::eA);
    MouseMovedEvent mouse_event(10.0, 20.0);

    dispatcher.dispatch(key_event);
    EXPECT_EQ(key_listener->event_count.load(), 1);
    EXPECT_EQ(mouse_listener->event_count.load(), 0);

    dispatcher.dispatch(mouse_event);
    EXPECT_EQ(key_listener->event_count.load(), 1);
    EXPECT_EQ(mouse_listener->event_count.load(), 1);
}

// ============================================================================
// EventBus Tests
// ============================================================================

TEST(EventBusTest, ProcessEvents) {
    EventBus bus;
    EventQueue queue;
    auto listener = std::make_shared<TestListener>();

    bus.registerListener(EventType::eKeyPressed, listener);

    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eB));
    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eC));

    bus.processEvents(queue);

    EXPECT_EQ(listener->event_count.load(), 3);
    EXPECT_TRUE(queue.empty());
}

TEST(EventBusTest, DispatchImmediate) {
    EventBus bus;
    auto listener = std::make_shared<TestListener>();

    bus.registerListener(EventType::eKeyPressed, listener);

    KeyPressedEvent event(KeyCode::eA);
    bus.dispatchImmediate(event);

    EXPECT_EQ(listener->event_count.load(), 1);
}

TEST(EventBusTest, MixedEventTypes) {
    EventBus bus;
    EventQueue queue;
    auto key_listener = std::make_shared<TestListener>();
    auto mouse_listener = std::make_shared<TestListener>();

    bus.registerListener(EventType::eKeyPressed, key_listener);
    bus.registerListener(EventType::eMouseMoved, mouse_listener);

    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    queue.push(std::make_unique<MouseMovedEvent>(10.0, 20.0));
    queue.push(std::make_unique<KeyPressedEvent>(KeyCode::eB));

    bus.processEvents(queue);

    EXPECT_EQ(key_listener->event_count.load(), 2);
    EXPECT_EQ(mouse_listener->event_count.load(), 1);
}

// ============================================================================
// EventManager Tests
// ============================================================================

TEST(EventManagerTest, Singleton) {
    auto& manager1 = EventManager::instance();
    auto& manager2 = EventManager::instance();

    EXPECT_EQ(&manager1, &manager2);
}

TEST(EventManagerTest, PushAndProcess) {
    auto& manager = EventManager::instance();
    auto listener = std::make_shared<TestListener>();

    manager.registerListener(EventType::eKeyPressed, listener);
    manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eA));

    EXPECT_GT(manager.pendingEventCount(), 0u);

    manager.processEvents();

    EXPECT_GE(listener->event_count.load(), 1);

    // Cleanup
    manager.unregisterListener(EventType::eKeyPressed, listener.get());
    manager.clearPendingEvents();
}

TEST(EventManagerTest, ClearPendingEvents) {
    auto& manager = EventManager::instance();

    manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eA));
    manager.pushEvent(std::make_unique<KeyPressedEvent>(KeyCode::eB));

    EXPECT_GT(manager.pendingEventCount(), 0u);

    manager.clearPendingEvents();

    EXPECT_EQ(manager.pendingEventCount(), 0u);
}

}  // namespace vne::events
