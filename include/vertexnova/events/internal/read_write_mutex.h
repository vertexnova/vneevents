#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2025-2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   January 2026
 *
 * Internal thread synchronization utilities for vneevents.
 * ----------------------------------------------------------------------
 */

#include <shared_mutex>

#include "../export.h"

namespace vne::events::internal {

/**
 * @class ReadWriteMutex
 * @brief Wrapper for std::shared_mutex with a simple interface.
 */
class VNEEVENTS_API ReadWriteMutex {
   public:
    void lock() { mutex_.lock(); }
    void unlock() { mutex_.unlock(); }
    bool tryLock() { return mutex_.try_lock(); }
    void lockShared() { mutex_.lock_shared(); }
    void unlockShared() { mutex_.unlock_shared(); }
    bool tryLockShared() { return mutex_.try_lock_shared(); }
    std::shared_mutex& get() { return mutex_; }

   private:
    std::shared_mutex mutex_;
};

/**
 * @class ReadLockGuard
 * @brief RAII guard for acquiring a shared (read) lock.
 */
class VNEEVENTS_API ReadLockGuard {
   public:
    explicit ReadLockGuard(ReadWriteMutex& m)
        : mutex_(m) {
        mutex_.lockShared();
    }
    ~ReadLockGuard() { mutex_.unlockShared(); }

    ReadLockGuard(const ReadLockGuard&) = delete;
    ReadLockGuard& operator=(const ReadLockGuard&) = delete;

   private:
    ReadWriteMutex& mutex_;
};

/**
 * @class WriteLockGuard
 * @brief RAII guard for acquiring an exclusive (write) lock.
 */
class VNEEVENTS_API WriteLockGuard {
   public:
    explicit WriteLockGuard(ReadWriteMutex& m)
        : mutex_(m) {
        mutex_.lock();
    }
    ~WriteLockGuard() { mutex_.unlock(); }

    WriteLockGuard(const WriteLockGuard&) = delete;
    WriteLockGuard& operator=(const WriteLockGuard&) = delete;

   private:
    ReadWriteMutex& mutex_;
};

}  // namespace vne::events::internal
