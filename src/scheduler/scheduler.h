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
public:
    DISALLOW_COPYING(Scheduler);

    Scheduler() :
            _downloader(nullptr) {}
    virtual ~Scheduler() = default;

    // insert more urls to fetch
    virtual void AddMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) = 0;

    virtual bool RunAndJoin() = 0;
    virtual void Stop() = 0;

    virtual void SetDownloader(fetcher::Downloader *downloader) {
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
                      _requestUrls(0),
                      _stop(false) {};

    ~FIFOScheduler() override = default;

    void AddMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) override;

    bool RunAndJoin() override;
    void Stop() override { _stop.store(true, std::memory_order_release); }

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
