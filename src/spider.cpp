#include <iostream>
#include <vector>

#include <glog/logging.h>

#include "common/disallow_coping.h"
#include "common/url_request.h"
#include "common/stdx/memory.h"
#include "scheduler/scheduler.h"
#include "spider/middleware/processor.h"

const std::string LOGS_DIR = "./logs/";

namespace spider {

using namespace scheduler;

class Crawler {
    DISALLOW_COPYING(Crawler);

public :
    Crawler(Scheduler *scheduler);
    ~Crawler() {};

    bool startup();

    // initialized seed urls. we will start crawling web page
    // from these points. And they are important to be specified.
    void useSeedUrls(std::vector<std::string> &seeds);

private :
    std::unique_ptr<Scheduler> _scheduler;

    // entire url seed list
    std::vector<std::string> _seedUrls;
};

Crawler::Crawler(Scheduler *scheduler) :
        _scheduler(scheduler) {
    LOG_ASSERT(_scheduler != nullptr);

    LOG(INFO) << "Gloal crawler initailize successful";
}

bool Crawler::startup() {
    // initialize seed urls that the point site we would start to crawl
    std::vector<std::unique_ptr<url::DownloadRequest>> initSeeds;
    for (auto &url : this->_seedUrls) {
        initSeeds.push_back(stdx::make_unique<url::DownloadRequest>(url));
    }

    _scheduler->setDownloader(std::make_shared<fetcher::ThreadPoolFetcher>(std::thread::hardware_concurrency() * 2));
    _scheduler->addMoreUrls(initSeeds);
    _scheduler->runAndJoin();
    _scheduler->stop();
    return true;
}

void Crawler::useSeedUrls(std::vector<std::string> &seeds) {
    this->_seedUrls = std::move(seeds);
}

}   // end of namespace spider


using namespace spider;

void readUrlSeeds(std::vector<std::string> &);
void startCrawler();

int main(int argc, char **argv) {
    // initialize glog
    FLAGS_log_dir = LOGS_DIR;
    const char *appName = static_cast<const char *>(argv[0]);
    google::InitGoogleLogging(appName);

    startCrawler();

    LOG(INFO) << "spider shutdown !!! \n";
    google::ShutdownGoogleLogging();
    return 0;
}

void readUrlSeeds(std::vector<std::string> &seeds) {
    seeds.push_back("http://www.baidu.com");
}

void startCrawler() {
    std::unique_ptr<spider::Crawler> spider = stdx::make_unique<spider::Crawler>(new FIFOScheduler());
    std::vector<std::string> urlSeeds;
    readUrlSeeds(urlSeeds);
    spider->useSeedUrls(urlSeeds);
    spider->startup();
}
