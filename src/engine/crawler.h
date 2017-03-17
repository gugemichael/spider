//
// Created by Michael on 15/03/2017.
//

#pragma once

#include "engine.h"
#include "scheduler/scheduler.h"
#include "spider/fetcher/download.h"

namespace spider {
namespace engine {

class GlobalCrawler {
    DISALLOW_COPYING(GlobalCrawler);

public :
    explicit GlobalCrawler(scheduler::Scheduler* scheduler);
    ~GlobalCrawler() {};

    bool startup();

    // initialized seed urls. we will start crawling web page
    // from these points. And they are important to be specified.
    void useSeedUrls(std::vector<std::string>& seeds);

private :
    std::shared_ptr<scheduler::Scheduler> _scheduler;
    std::shared_ptr<fetcher::Downloader> _downloader;

    // unique global spider engine
    std::unique_ptr<GlobalEngine> _engine;

    // entire url seed list
    std::vector<std::string> _seeds;
};

}   // end of namespace engine
}   // end of namespace spider

