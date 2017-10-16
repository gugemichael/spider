//
// Created by Michael on 15/03/2017.
//

#include "utils/log.h"
#include "utils/util.h"

#include "engine/crawler.h"

namespace spider {
namespace engine {

GlobalSpider::GlobalSpider(scheduler::Scheduler *scheduler) :
        _scheduler(scheduler),
        _engine(stdx::make_unique<GlobalEngine>()) {
    invariant(_scheduler != nullptr);

    LogInfo("Gloal crawler initailize successful");
}

bool GlobalSpider::startup() {
    // initialize seed urls that the point site we would start to crawl
    std::vector<std::unique_ptr<http::DownloadRequest>> seeds;
    for (auto& url : this->_seeds) {
        seeds.push_back(stdx::make_unique<http::DownloadRequest>(url));
    }

    _downloader = std::make_shared<fetcher::ThreadPoolFetcher>(_engine.get(), std::thread::hardware_concurrency() * 2);
    _scheduler->SetDownloader(_downloader.get());
    _scheduler->AddMoreUrls(seeds);

    _engine->UseScheduler(_scheduler);
    return _engine->Startup();
}

void GlobalSpider::setSeedUrls(std::vector<std::string>& seeds) {
    this->_seeds = std::move(seeds);
}

}
}
