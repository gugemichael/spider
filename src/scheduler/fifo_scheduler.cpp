#include <glog/logging.h>


#include "scheduler.h"
#include "common/basic.h"
#include "common/url_request.h"

namespace spider {
namespace scheduler {

void FIFOScheduler::addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) {
    for (auto& url : urls) {
        this->_requestQueue.blockingWrite(url.release());
    }

    this->_requestUrls += urls.size();
}

void FIFOScheduler::runAndJoin() {
    invariant(_downloader != nullptr);

    _poller = std::thread(std::bind(&FIFOScheduler::schedule, this));
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (_poller.joinable())
        _poller.join();
}

void FIFOScheduler::schedule() {
    url::DownloadRequest* job;

    while (!_stop.load(std::memory_order_acquire)) {
        this->_requestQueue.blockingRead(job);
        LOG(INFO) << "schedule transfer download task. url -> " << job->uri();

        this->_downloader->addTask(job, [](url::DownloadRequest* request, fetcher::DownloadResponse* response) {
            switch (response->status) {
                case fetcher::DownloadStatus::SUCCESS:
                    LOG(INFO) << "url request download success. url [" << request->uri() << "]";
                    break;
                case fetcher::DownloadStatus::NETWORK_FAILED:
                case fetcher::DownloadStatus::REQUEST_INVALID:
                    LOG(WARNING) << "url request download error : " << int(response->status) << ". url ["
                                 << request->uri()
                                 << "]";
                    break;
            }
        });
    }

    LOG(INFO) << "scheduler request queue empty and exit ...";
}

}
}
