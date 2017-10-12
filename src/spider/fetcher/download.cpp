//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_CPP
//

#include <iostream>
#include "utils/log.h"
#include "utils/util.h"

#include "engine/engine.h"

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

url::WebPageObject *parse(WebParser *parser, const std::string& url, const std::string& content) {
    auto web = stdx::make_unique<url::WebPageObject>(url);
    parser->parse(content, web.get());

    return web.release();
}

void ThreadPoolFetcher::worker(int id) {
    LogInfo("thread pool download worker %d startup with lazy_queue length : %u", id, _taskQueue.capacity());
    std::string return_body;

    httpClient.SetTimeout(30);
    httpClient.Initail();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        return_body.clear();
        std::unique_ptr<Downloadable> task(this->_taskQueue.take());

        auto resp = stdx::make_unique<DownloadResponse>();
        resp->status = DownloadStatus::SUCCESS;

        switch (task->request->httpMethod()) {
            case url::HttpMethod::GET:
                if (!httpClient.RequestGet(task->request->uri(), return_body)) {
                    LogWarning("http request failed %s", httpClient.GetLastError());
                    resp->status = DownloadStatus::REQUEST_FAIL;
                }
                break;
            case url::HttpMethod::POST:
                break;
        }
        resp->bodyContent = std::move(return_body);

        if (task->callback)
            task->callback(task->request, resp.get());

        auto web_page = parse(_httpUrlsExtractor.get(), task->request->uri(), resp->bodyContent);

        // engine::process() must be the lasted step. so others could
        // not take the ownership with webObject
        _engine->OnRequestComplete(web_page);

        LogInfo("thread pool downloader fetch web url %s, content size %s", task->request->uri().c_str(),
                utils::human_readable_capacity(resp->bodyContent.size()).c_str());
    }
#pragma clang diagnostic pop
}


void ThreadPoolFetcher::Destory() {

}

}
}

