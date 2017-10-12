#include "utils/log.h"
#include "utils/util.h"
#include "common/web/url_request.h"

#include "scheduler.h"

namespace spider {
namespace scheduler {

void FIFOScheduler::AddMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) {
    for (auto& url : urls)
        this->_requestQueue.offer(url.release());

    this->_requestUrls += urls.size();
}

bool FIFOScheduler::RunAndJoin() {
    invariant(_downloader != nullptr);

    _poller = std::thread(std::bind(&FIFOScheduler::schedule, this));
    if (_poller.joinable())
        _poller.join();

    return true;
}

void FIFOScheduler::schedule() {
    url::DownloadRequest *job;

    while (!_stop.load(std::memory_order_acquire)) {
        job = this->_requestQueue.take();
        LogDebug("schedule transfer download task. url -> %s", job->uri().c_str());

        this->_downloader->AddTask(job, [](url::DownloadRequest *requestJob, fetcher::DownloadResponse *response) {
            switch (response->status) {
                case fetcher::DownloadStatus::SUCCESS:
                    break;
                case fetcher::DownloadStatus::NETWORK_FAILED:
                case fetcher::DownloadStatus::REQUEST_INVALID:
                case fetcher::DownloadStatus::REQUEST_FAIL:
                    LogWarning("url request download error : %d, url[%s]", int(response->status),
                               requestJob->uri().c_str());
                    break;
                case fetcher::DownloadStatus::UNKNOWN_ERROR:
                    break;
            }
        });
    }

    LogInfo("scheduler request queue empty and exit ...");
}

}
}
