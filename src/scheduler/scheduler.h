//
// Created by Michael on 10/17/16.
//
#pragma once

#include <string>
#include <vector>

#include <boost/lockfree/queue.hpp>
#include <common/url_request.h>
#include <thread>

#include "common/disallow_coping.h"

namespace spider {
namespace scheduler {

class Scheduler {
    DISALLOW_COPYING(Scheduler);

public:
    Scheduler() = default;
    virtual ~Scheduler() = default;

    // insert more urls to fetch
    virtual void addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) = 0;

    virtual void run() = 0;
};

class FIFOScheduler : public Scheduler {

public:
    FIFOScheduler() : Scheduler(),
                      _requestQueue(4096),
                      _poller(std::bind(&FIFOScheduler::schedule, this)) {};

    ~FIFOScheduler() {};

    virtual void addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) override;

    virtual void run() override;

    void schedule();

private:
    // multi thread safe queue keep the request from out side. the
    // downloader will fetch request able object from this queue
    boost::lockfree::queue<url::DownloadRequest*> _requestQueue;

    // schedule thread
    std::thread _poller;

    std::atomic<long> _requestUrls;
};

}
}
