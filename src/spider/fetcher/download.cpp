//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_CPP
//

#include "common/url_request.h"

#include "download.h"

namespace fetcher {

void ThreadPoolFetcher::addTask(url::DownloadRequest *task, DownloadCallbackFunc func) {
    auto obeject = stdx::make_unique<DownloadObject>(task);
    obeject->setCallback(func);

    _taskQueue.offer(obeject.release());
}

void ThreadPoolFetcher::__worker(int id) {
    LOG(INFO) << "thread pool download worker " << id << " startup";
    std::string returnBody;

    while (true) {
        DownloadObject *task = this->_taskQueue.take();

        auto resp = stdx::make_unique<DownloadResponse>();
        resp->status = DownloadStatus::SUCCESS;

        switch (task->request->httpMethod()) {
            case url::HttpMethod::GET:
                if (!_curl->doGet(task->request->uri(), returnBody))
                    resp->status = DownloadStatus::REQUEST_INVALID;
                break;
            case url::HttpMethod::POST:
                break;
        }
        resp->body = std::move(returnBody);

        if (task->callback)
            task->callback(task->request, resp.get());

        LOG(INFO) << "thread pool downloader fetch content size " << resp->body.size();
    }
}
void ThreadPoolFetcher::destory() {

}
}

