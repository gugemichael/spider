#include <iostream>
#include <vector>

#include <glog/logging.h>
#include <engine/crawler.h>

#include "common/disallow_coping.h"
#include "common/web/url_request.h"
#include "common/stdx/memory.h"
#include "scheduler/scheduler.h"
#include "spider/middleware/processor.h"

using namespace spider;

const std::string LOGS_DIR = "./logs/";

void readUrlSeeds(std::vector<std::string> &);
void startCrawler();

int main(int argc, char **argv) {
    // initialize glog
    FLAGS_log_dir = LOGS_DIR;
    const char *appName = static_cast<const char *>(argv[0]);
    google::InitGoogleLogging(appName);

    startCrawler();

    LOG(INFO) << "spider shutdown !!! ";
    google::ShutdownGoogleLogging();
    return 0;
}

void readUrlSeeds(std::vector<std::string> &seeds) {
    seeds.push_back("http://www.qq.com");
    seeds.push_back("http://www.baidu.com");
    seeds.push_back("http://www.csdn.net");
    seeds.push_back("http://www.163.com");
    seeds.push_back("http://www.sina.com.cn");
    seeds.push_back("http://www.cplusplus.com");
}

void startCrawler() {
    std::unique_ptr<engine::GlobalCrawler> crawler =
            stdx::make_unique<engine::GlobalCrawler>(new scheduler::FIFOScheduler());
    std::vector<std::string> urlSeeds;
    readUrlSeeds(urlSeeds);
    crawler->useSeedUrls(urlSeeds);
    crawler->startup();

    LOG(INFO) << "crawler worker complete and exit ";
}
