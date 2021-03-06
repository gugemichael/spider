//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_H
//

#pragma once

#include <queue>
#include <string>
#include <array>
#include <thread>

#include <glog/logging.h>

#include "common/stdx/memory.h"
#include "common/web/url_request.h"
#include "common/block_queue.h"

#include "spider/fetcher/web_parser.h"
#include "spider/fetcher/simple_curl.h"


namespace spider {

namespace engine {
class GlobalEngine;
}

namespace fetcher {

enum class DownloadStatus {
    SUCCESS, NETWORK_FAILED, REQUEST_INVALID, UNKNOWN_ERROR
};

struct DownloadResponse {
    DownloadStatus status;
    std::string bodyContent;
};

using DownloadCallbackFunc = std::function<void(url::DownloadRequest*, DownloadResponse*)>;
using UniversalParser = std::array<std::shared_ptr<WebParser>, ALL_PARSER_COUNT>;

using namespace spider;

/**
 * Url request downloader
 *
 */
class Downloader {

public:
    Downloader(engine::GlobalEngine* engine) :
            _engine(engine),
            _httpParsers{std::make_shared<HeaderParser>(), std::make_shared<PageContentParser>()} {

    }
    virtual ~Downloader() = default;

    virtual void addTask(url::DownloadRequest* task, DownloadCallbackFunc func = nullptr) = 0;
    virtual void destory() = 0;

    struct Downloadable {
        Downloadable(url::DownloadRequest* req) : request(req) {}
        void setCallback(DownloadCallbackFunc proc) { this->callback = proc; }

        url::DownloadRequest* request;
        DownloadCallbackFunc callback;
    };

protected:
    // global engine
    engine::GlobalEngine* _engine;      // not owned

    // specific parser
    UniversalParser _httpParsers;
};


class ThreadPoolFetcher : public Downloader {
    using ThreadGroup = std::vector<std::thread>;

public:
    ThreadPoolFetcher(engine::GlobalEngine* engine, uint32_t threads) :
            Downloader(engine),
            _threads(threads),
            _taskQueue(4096) {

        for (uint32_t i = 0; i < this->_threads; ++i) {
            _threadPool.emplace_back(std::bind(&ThreadPoolFetcher::__worker, this, i));
        }
    }

    virtual void addTask(url::DownloadRequest* task, DownloadCallbackFunc func = nullptr) override;

    void destory() override;

    void __worker(int id);

private:
    // thread pool and attached blocking queue
    uint32_t _threads;
    ThreadGroup _threadPool;
    spider::LazyNotifyQueue<Downloadable*> _taskQueue;

    // libcurl handle
    static thread_local SimpleCUrl _curl;
};

}   // end of namespace fetcher
}   // end of namespace spider

