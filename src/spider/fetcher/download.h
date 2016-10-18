//
// Created by Michael on 10/17/16.
//
// SPIDER_DOWNLOADER_H
//

#pragma once

#include <queue>
#include <string>

namespace fetcher {

/**
 * Url Term downloader
 *
 */
class Downloader {

public:
    void addScheduledJob(const Term &term);

private:
    std::queue<Term> _jobs;
};


/**
 * URL represent information
 *
 */
class Term {

private:
    std::string _url;
};

}   // end of namespace fetcher

