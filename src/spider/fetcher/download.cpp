//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_CPP
//

#include <common/web/web_object.h>
#include "engine/engine.h"
#include "common/web/url_request.h"

#include "download.h"

namespace spider {
namespace fetcher {

using namespace spider;

thread_local SimpleCUrl ThreadPoolFetcher::_curl;

void ThreadPoolFetcher::addTask(url::DownloadRequest* task, DownloadCallbackFunc func) {
    auto obeject = stdx::make_unique<Downloadable>(task);
    obeject->setCallback(func);

    // don't reclaim url::DownloadRequest* of task object. even
    // on exception happen
    _taskQueue.offer(obeject.release());
}

url::WebObject* __parse(const UniversalParser& parsers, const std::string& url, const std::string& content) {
    auto web = stdx::make_unique<url::WebObject>(url);
    for (auto& parser : parsers) {
        parser->parse(web.get());
    }

    return web.release();
}

void ThreadPoolFetcher::__worker(int id) {
    LOG(INFO) << "thread pool download worker " << id << " startup with lazy_queue length : " << _taskQueue.capacity();
    std::string returnBody;

    _curl.setTimeout(30);
    _curl.initialize();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        std::unique_ptr<Downloadable> task(this->_taskQueue.take());

        auto resp = stdx::make_unique<DownloadResponse>();
        resp->status = DownloadStatus::SUCCESS;

        switch (task->request->httpMethod()) {
            case url::HttpMethod::GET:
                if (!_curl.doGet(task->request->uri(), returnBody))
                    resp->status = DownloadStatus::UNKNOWN_ERROR;
                break;
            case url::HttpMethod::POST:
                break;
        }
        resp->bodyContent = std::move(returnBody);

        if (task->callback)
            task->callback(task->request, resp.get());

        auto webObject = __parse(_httpParsers, task->request->uri(), resp->bodyContent);

        // engine::process() must be the lasted step. so others could
        // not take the ownership with webObject
        _engine->process(webObject);

        LOG(INFO) << "thread pool downloader fetch content size " << resp->bodyContent.size();
    }
#pragma clang diagnostic pop
}


void ThreadPoolFetcher::destory() {

}

}
}

