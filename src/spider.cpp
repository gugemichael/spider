
#include "scheduler/scheduler.h"
#include "spider/pipeline/pipeline.h"

#include <iostream>
#include <list>

#include "glog/logging.h"

namespace spider {

using Scheduler = scheduler::Scheduler;
using Pipeline = pipeline::Pipeline;

class Spider {

public :
    static std::string name() { return "spieder"; }

    Spider(std::shared_ptr<Scheduler> scheduler);
    Spider(Spider &spider) = delete;
    Spider(const Spider &spider) = delete;
    Spider(Spider &&spider) = delete;

    bool startupCrawl();

    // gived seed urls
    void seedUrl(const std::list<std::string> &seeds);

private :

    std::shared_ptr<Pipeline> _pipeline;
    std::shared_ptr<Scheduler> _scheduler;

    // seeds
    std::list<std::string> _seedUrls;
};

Spider::Spider(std::shared_ptr<Scheduler> scheduler) :
        _pipeline(std::make_shared<Pipeline>()),
        _scheduler(std::move(scheduler)) {
    LOG_ASSERT(_scheduler != nullptr);
    LOG_ASSERT(_pipeline != nullptr);

    LOG(ERROR) << "aaaaaaaa";
    LOG(INFO) << this->name() << " initailized successful !";
    LOG(WARNING) << this->name() << " initailized successful !";
}

bool Spider::startupCrawl() {

    return true;
}

void Spider::seedUrl(const std::list<std::string> &seeds) {
    this->_seedUrls = std::move(seeds);
}


}   // end of namespace spider


int main(int argc, char **argv) {
    // initialize glog
    const char *appName = static_cast<const char *>(argv[0]);
    google::InitGoogleLogging(appName);
    google::SetLogDestination(google::INFO, "spider.log.");

//    using namespace spider;
//
//
//    Spider *spider = new Spider(std::make_shared<scheduler::FIFOScheduler>());
//    spider->startupCrawl();
//
//    delete spider;

    VLOG(100) << "VLOG INFO 100";

    LOG(ERROR) << "spider shutdown !!! \n";
    google::ShutdownGoogleLogging();

    return 0;
}
