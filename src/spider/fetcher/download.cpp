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

thread_local SimpleHttpClient ThreadPoolFetcher::httpClient;

void ThreadPoolFetcher::AddTask(url::DownloadRequest *task, DownloadCallbackFunc func) {
    auto object = stdx::make_unique<Downloadable>(task);
    object->setCallback(func);

    // don't reclaim url::DownloadRequest* of task object. even
    // on exception happen
    _taskQueue.offer(object.release());
}

url::WebPageObject *parse(const UniversalParser& parsers, const std::string& url, const std::string& content) {
    auto web = stdx::make_unique<url::WebPageObject>(url);
    for (auto& parser : parsers) {
        parser->parse(web.get());
    }

    return web.release();
}

void ThreadPoolFetcher::worker(int id) {
    LOG(INFO) << "thread pool download worker " << id << " startup with lazy_queue length : " << _taskQueue.capacity();
    std::string returnBody;

    httpClient.setTimeout(30);
    httpClient.initialize();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        returnBody.clear();
        std::unique_ptr<Downloadable> task(this->_taskQueue.take());

        auto resp = stdx::make_unique<DownloadResponse>();
        resp->status = DownloadStatus::SUCCESS;

        switch (task->request->httpMethod()) {
            case url::HttpMethod::GET:
                if (!httpClient.httpGet(task->request->uri(), returnBody))
                    resp->status = DownloadStatus::UNKNOWN_ERROR;
                break;
            case url::HttpMethod::POST:
                break;
        }
        resp->bodyContent = std::move(returnBody);

        if (task->callback)
            task->callback(task->request, resp.get());

        auto webPage = parse(_httpParsers, task->request->uri(), resp->bodyContent);

        // engine::process() must be the lasted step. so others could
        // not take the ownership with webObject
        _engine->OnRequestComplete(webPage);

        LOG(INFO) << "thread pool downloader fetch content size " << resp->bodyContent.size();
    }
#pragma clang diagnostic pop
}


void ThreadPoolFetcher::Destory() {

}

}
}

