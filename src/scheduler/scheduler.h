//
// Created by Michael on 10/17/16.
//
#pragma once

#include <string>
#include <thread>
#include <vector>

#include "spider/fetcher/download.h"

#include "common/web/url_request.h"
#include "common/block_queue.h"
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

    virtual void runAndJoin() = 0;
    virtual void stop() = 0;

    virtual void setDownloader(fetcher::Downloader* downloader) {
        this->_downloader = downloader;
    }

protected:

    // web http downloader
    fetcher::Downloader* _downloader;   // not owned
};

class FIFOScheduler : public Scheduler {

public:
    FIFOScheduler() : Scheduler(),
                      _requestQueue(4096),
                      _stop(false) {};

    ~FIFOScheduler() {};

    virtual void addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) override;

    virtual void runAndJoin() override;
    virtual void stop() override { _stop.store(true, std::memory_order_release); }

    void schedule();

private:
    // multi thread safe queue keep the request from out side. the
    // downloader will fetch request able object from this queue

    // boost::lockfree::queue<url::DownloadRequest*> _requestQueue;
    LazyNotifyQueue<url::DownloadRequest*> _requestQueue;


    // schedule thread
    std::thread _poller;

    std::atomic<long> _requestUrls;
    std::atomic<bool> _stop;
};

}
}
