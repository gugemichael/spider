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
#include <utility>

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
    SUCCESS, NETWORK_FAILED, REQUEST_INVALID, REQUEST_FAIL, UNKNOWN_ERROR
};

struct DownloadResponse {
    DownloadStatus status;
    std::string bodyContent;
};

using DownloadCallbackFunc = std::function<void(http::DownloadRequest *, DownloadResponse *)>;

using namespace spider;

/**
 * Url request downloader
 *
 */
class Downloader {

public:
    explicit Downloader(engine::GlobalEngine *engine) :
            _engine(engine),
            _httpUrlsExtractor(std::make_shared<WebParserImpl>()) {}


    virtual ~Downloader() = default;

    virtual void AddTask(http::DownloadRequest *task, DownloadCallbackFunc func = nullptr) = 0;

    virtual void Destory() = 0;

    struct Downloadable {
        explicit Downloadable(http::DownloadRequest *req) : request(req) {}

        void setCallback(DownloadCallbackFunc proc) { this->callback = std::move(proc); }

        http::DownloadRequest *request;
        DownloadCallbackFunc callback;
    };

protected:
    // global engine
    engine::GlobalEngine *_engine;      // not owned

    // html parser
    std::shared_ptr<WebParser> _httpUrlsExtractor;
};


class ThreadPoolFetcher : public Downloader {
    using ThreadGroup = std::vector<std::thread>;

public:
    ThreadPoolFetcher(engine::GlobalEngine *engine, uint32_t threads) :
            Downloader(engine),
            _threads(threads),
            _taskQueue(4096) {

        for (uint32_t i = 0; i < this->_threads; ++i) {
            _threadPool.emplace_back(std::bind(&ThreadPoolFetcher::worker, this, i));
        }
    }

    void AddTask(http::DownloadRequest *task, DownloadCallbackFunc func = nullptr) override;

    void Destory() override;

    void worker(int id);

private:
    // thread pool and attached blocking queue
    uint32_t _threads;
    ThreadGroup _threadPool;
    spider::LazyNotifyQueue<Downloadable *> _taskQueue;

    // libcurl handle
    static thread_local SimpleHttpClient httpClient;
};

}   // end of namespace fetcher
}   // end of namespace spider

