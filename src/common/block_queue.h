//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <condition_variable>
#include <boost/lockfree/queue.hpp>

#include "common/disallow_coping.h"

namespace spider {

template<typename ElemType>
class LazyNotifyQueue {
public:
    DISALLOW_COPYING(LazyNotifyQueue);

    explicit LazyNotifyQueue(uint32_t capacity) :
            _internalCapacity(capacity),
            _internal(capacity) {}
    ~LazyNotifyQueue() = default;

    void offer(ElemType element) {
        _internal.push(element);
        _notifier.notify_all();
    }

    ElemType take() {
        ElemType got;
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
    boost::lockfree::queue<ElemType> _internal;

    // syncer
    std::mutex _lock;
    std::condition_variable _notifier;
};

}
