#include <iostream>
#include <vector>

#include <glog/logging.h>

#include "common/disallow_coping.h"
#include "common/url_request.h"
#include "scheduler/scheduler.h"
#include "spider/pipeline/pipeline.h"

const std::string LOGS_DIR = "./logs/";

namespace spider {

using namespace scheduler;

class Crawler {
    DISALLOW_COPYING(Crawler);

public :
    Crawler(Scheduler* scheduler);
    ~Crawler() {};

    bool startup();

    // initialized seed urls. we will start crawling web page
    // from these points. And they are important to be specified.
    void useSeedUrls(std::vector<std::string>& seeds);

private :
    std::unique_ptr<Scheduler> _scheduler;

    // entire url seed list
    std::vector<std::string> _seedUrls;
};

Crawler::Crawler(Scheduler* scheduler) :
        _scheduler(scheduler) {
    LOG_ASSERT(_scheduler != nullptr);

    LOG(INFO) << "Gloal crawler initailize successfully !";
}

bool Crawler::startup() {
    std::vector<std::unique_ptr<url::DownloadRequest>> initSeeds;
    for (auto& url : this->_seedUrls) {
        initSeeds.push_back(std::make_unique<url::DownloadRequest>(url));
    }

    _scheduler->addMoreUrls(initSeeds);
    _scheduler->run();
    return true;
}

void Crawler::useSeedUrls(std::vector<std::string>& seeds) {
    this->_seedUrls = std::move(seeds);
}

}   // end of namespace spider


using namespace spider;

void readUrlSeeds(std::vector<std::string>&);

int main(int argc, char** argv) {
    // initialize glog
    FLAGS_log_dir = LOGS_DIR;
    const char* appName = static_cast<const char*>(argv[0]);
    google::InitGoogleLogging(appName);

    std::unique_ptr<spider::Crawler> spider = std::make_unique<spider::Crawler>(new FIFOScheduler());

    std::vector<std::string> urlSeeds;
    readUrlSeeds(urlSeeds);
    spider->useSeedUrls(urlSeeds);
    spider->startup();

    LOG(INFO) << "spider shutdown !!! \n";
    google::ShutdownGoogleLogging();

    return 0;
}

void readUrlSeeds(std::vector<std::string>& seeds) {
    seeds.push_back("http://www.baidu.com");
}
