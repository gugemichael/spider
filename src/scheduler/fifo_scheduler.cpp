#include <glog/logging.h>


#include "scheduler.h"
#include "common/basic.h"
#include "common/web/url_request.h"

namespace spider {
namespace scheduler {

void FIFOScheduler::addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) {
    for (auto& url : urls) {
        this->_requestQueue.offer(url.release());
    }

    this->_requestUrls += urls.size();
}

void FIFOScheduler::runAndJoin() {
    invariant(_downloader != nullptr);

    _poller = std::thread(std::bind(&FIFOScheduler::schedule, this));
    if (_poller.joinable())
        _poller.join();
}

void FIFOScheduler::schedule() {
    url::DownloadRequest* job;

    while (!_stop.load(std::memory_order_acquire)) {
        job = this->_requestQueue.take();
        LOG(INFO) << "schedule transfer download task. url -> " << job->uri();

        this->_downloader->addTask(job, [](url::DownloadRequest* requestJob, fetcher::DownloadResponse* response) {
            std::unique_ptr<url::DownloadRequest> release(requestJob);
            switch (response->status) {
                case fetcher::DownloadStatus::SUCCESS:
                    LOG(INFO) << "url request download success. url [" << release->uri() << "]";
                    break;
                case fetcher::DownloadStatus::NETWORK_FAILED:
                case fetcher::DownloadStatus::REQUEST_INVALID:
                case fetcher::DownloadStatus::UNKNOWN_ERROR:
                    LOG(WARNING) << "url request download error : " << int(response->status) << ". url ["
                                 << release->uri()
                                 << "]";
                    break;
            }
        });
    }

    LOG(INFO) << "scheduler request queue empty and exit ...";
}

}
}
