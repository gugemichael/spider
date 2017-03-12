#include <glog/logging.h>

#include "common/url_request.h"
#include "scheduler.h"

namespace spider {
namespace scheduler {

void FIFOScheduler::addMoreUrls(std::vector<std::unique_ptr<url::DownloadRequest>>& urls) {
//    for (std::vector<std::unique_ptr<url::DownloadRequest>>::iterator iter = std::begin(urls);
//         iter != std::end(urls); ++iter) {
//        this->_requestQueue.push(iter->release());
//    }

    for (auto& url : urls) {
        this->_requestQueue.push(url.release());
    }

    this->_requestUrls += urls.size();
}

void FIFOScheduler::run() {
    _poller.join();
}

void FIFOScheduler::schedule() {
    url::DownloadRequest* job;

    while (this->_requestQueue.pop(job)) {
        LOG(INFO) << "scheduler receive job !";
    }

    LOG(INFO) << "scheduler request queue empty and exit .";
}

}
}
