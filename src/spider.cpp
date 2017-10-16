#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <fstream>

#include "utils/log.h"

#include "engine/crawler.h"

using namespace spider;

const std::string SPIDER = "/tmp/spider";
const std::string SPIDER_LOGS_DIR = SPIDER + "/logs/";
const std::string SPIDER_URLS = SPIDER + "/urls";

std::vector<std::string> readUrlSeeds() {
    char buffer[8192];
    std::vector<std::string> urls;
    if (access(SPIDER_URLS.c_str(), 0) == 0) {
        std::ifstream reader;
        reader.open(SPIDER_URLS, std::ios::in);
        while (reader) {
            memset(buffer, 0, sizeof(buffer));
            reader.getline(buffer, sizeof(buffer));
            std::string url(buffer);
            if (!url.empty() && url.at(0) != '#')
                urls.push_back(url);

//            if (strnlen(buffer, sizeof(buffer)))
//                urls.emplace_back(buffer);
        }
    }

    LogInfo("successful load http seeds count %d", urls.size());
    return std::move(urls);
}

int main(int argc, char **argv) {
    // create spider global folder
    if (access(SPIDER.c_str(), 2) != 0) {
        if (mkdir(SPIDER.c_str(), S_IRWXU | S_IRGRP | S_IWGRP | S_IRWXO) != 0) {
            fprintf(stderr, "make spider global directory %s failed.", SPIDER.c_str());
            exit(-1);
        }
    }

//    auto logger = nimo_log_split_init(SPIDER_LOGS_DIR.c_str(), SPLIT_ONE_DAY, 0);
    auto logger = nimo_log_split_init(nullptr, SPLIT_ONE_DAY, 0);
    nimo_log_buffer(logger, 8192);
    nimo_log_level(logger, INFO);

    // initialize global spider and scheduler engine
    engine::GlobalSpider *crawler = new engine::GlobalSpider(new scheduler::FIFOScheduler());
    if (!(crawler->logger = logger)) {
        fprintf(stderr, "initialize global logger failed");
        exit(-1);
    }

    std::vector<std::string> seeds = readUrlSeeds();
    crawler->setSeedUrls(seeds);
    crawler->startup();

    LogInfo("crawler worker complete and exit ");

    delete crawler;
    nimo_log_destroy();

    return 0;
}

