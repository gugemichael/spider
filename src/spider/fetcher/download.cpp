//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_CPP
//

#include "common/web/url_request.h"

#include "download.h"

namespace spider {
namespace fetcher {

thread_local SimpleCUrl ThreadPoolFetcher::_curl;

void ThreadPoolFetcher::addTask(url::DownloadRequest *task, DownloadCallbackFunc func) {
    auto obeject = stdx::make_unique<Downloadable>(task);
    obeject->setCallback(func);

    _taskQueue.offer(obeject.release());
}

void ThreadPoolFetcher::__worker(int id) {
    LOG(INFO) << "thread pool download worker " << id << " startup with lazy_queue length : " << _taskQueue.capacity();
    std::string returnBody;

    _curl.setTimeout(30);
    _curl.initialize();

    while (true) {
        Downloadable *task = this->_taskQueue.take();

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
        resp->body = std::move(returnBody);

        if (task->callback)
            task->callback(task->request, resp.get());

        delete task;

        LOG(INFO) << "thread pool downloader fetch content size " << resp->body.size();
    }
}

void ThreadPoolFetcher::destory() {

}

}
}

