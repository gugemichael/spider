//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <condition_variable>
#include <boost/lockfree/queue.hpp>

#include "common/disallow_coping.h"

namespace spider {

template<typename T>
class LazyPendingQueue {
    DISALLOW_COPYING(LazyPendingQueue);

public:
    LazyPendingQueue(uint32_t capacity) :
            _internalCapacity(capacity),
            _internal(capacity) {}
    ~LazyPendingQueue() = default;

    void offer(T element) {
        _internal.push(element);
        _notifier.notify_all();
    }

    T take() {
        T got;
        while (!_internal.pop(got)) {
            std::unique_lock<std::mutex> locker(_lock);
            _notifier.wait(locker);
        }

        return got;
    }

    uint32_t capacity() { return _internalCapacity; }

private:
    // internal queue
    uint32_t _internalCapacity;
    boost::lockfree::queue<T> _internal;
    // syncer
    std::mutex _lock;
    std::condition_variable _notifier;
};

}
