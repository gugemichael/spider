//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_CPP
//

#include "common/url_request.h"
#include "download.h"

namespace fetcher {

void ThreadPoolFetcher::addTask(url::DownloadRequest* task, DownloadCallbackFunc func) {
    auto obeject = std::make_unique<DownloadObject>(task);
    obeject->setCallback(func);

    _taskQueue.blockingWrite(obeject.release());
}

void ThreadPoolFetcher::__worker(int id) {
    LOG(INFO) << "thread pool download worker " << id << " startup";
    DownloadObject* task = nullptr;
    std::string returnBody;

    while (true) {
        this->_taskQueue.blockingRead(task);

        auto resp = std::make_unique<DownloadResponse>();
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

