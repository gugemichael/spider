//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_H
//

#pragma once

#include <queue>
#include <string>
#include <thread>

#include <glog/logging.h>

#include "common/url_request.h"
#include "common/block_queue.h"
#include "common/stdx/memory.h"

#include "spider/fetcher/simple_curl.h"

namespace fetcher {

using namespace engine;

enum class DownloadStatus {
    SUCCESS, NETWORK_FAILED, REQUEST_INVALID
};

struct DownloadResponse {
    DownloadStatus status;
    std::string body;
};

using DownloadCallbackFunc = std::function<void(url::DownloadRequest *, DownloadResponse *)>;

/**
 * Url request downloader
 *
 */
class Downloader {

public:
    virtual void addTask(url::DownloadRequest *task, DownloadCallbackFunc func = nullptr) = 0;
    virtual void destory() = 0;

    struct DownloadObject {
        DownloadObject(url::DownloadRequest *req) : request(req) {}
        void setCallback(DownloadCallbackFunc proc) { this->callback = proc; }

        url::DownloadRequest *request;
        DownloadCallbackFunc callback;
    };
};


class ThreadPoolFetcher : public Downloader {
    using ThreadGroup = std::vector<std::thread>;

public:
    ThreadPoolFetcher(uint32_t threads) :
            _threads(threads),
            _curl(stdx::make_unique<SimpleCUrl>()),
            _taskQueue(4096) {

        _curl->setTimeout(30);
        _curl->initialize();

        for (uint32_t i = 0; i < this->_threads; ++i) {
            _threadPool.emplace_back(std::bind(&ThreadPoolFetcher::__worker, this, i));
        }
    }

    virtual void addTask(url::DownloadRequest *task, DownloadCallbackFunc func = nullptr) override;

    void destory() override;

    void __worker(int id);

private:
    // thread pool and attached blocking queue
    uint32_t _threads;
    ThreadGroup _threadPool;
    engine::LazyPendingQueue<DownloadObject *> _taskQueue;

    // libcurl handle
    std::unique_ptr<SimpleCUrl> _curl;
};

}   // end of namespace fetcher

