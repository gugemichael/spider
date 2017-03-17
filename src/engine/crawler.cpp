//
// Created by Michael on 15/03/2017.
//

#include "crawler.h"

namespace spider {
namespace engine {

GlobalCrawler::GlobalCrawler(scheduler::Scheduler* scheduler) :
        _scheduler(scheduler) {
    LOG_ASSERT(_scheduler != nullptr);

    LOG(INFO) << "Gloal crawler initailize successful";
}

bool GlobalCrawler::startup() {
    // initialize seed urls that the point site we would start to crawl
    std::vector<std::unique_ptr<url::DownloadRequest>> seeds;
    for (auto& url : this->_seeds) {
        seeds.push_back(stdx::make_unique<url::DownloadRequest>(url));
    }

    _engine = stdx::make_unique<GlobalEngine>();
    _engine->setScheduler(_scheduler);
    _downloader = std::make_shared<fetcher::ThreadPoolFetcher>(_engine.get(), std::thread::hardware_concurrency() * 2);
    _scheduler->setDownloader(_downloader.get());
    _scheduler->addMoreUrls(seeds);
    _scheduler->runAndJoin();
    _scheduler->stop();
    return true;
}

void GlobalCrawler::useSeedUrls(std::vector<std::string>& seeds) {
    this->_seeds = std::move(seeds);
}

}
}
