//
// Created by Michael on 15/03/2017.
//

#pragma once

#include <mutex>
#include <curl/curl.h>
#include <unordered_map>

#include "common/disallow_coping.h"

constexpr int DEFAULT_MAX_RESPONSE_SIZE = 16 * 1024;

using HttpHeader = std::unordered_map<std::string, std::string>;

namespace spider {
namespace fetcher {

class SimpleCUrl {
    DISALLOW_COPYING(SimpleCUrl);

public:
    SimpleCUrl() :
            _maxResponseSize(DEFAULT_MAX_RESPONSE_SIZE),
            _timeout(30) {}
    ~SimpleCUrl() = default;

    bool initialize();
    bool hasError() { return _error != 0; }

    void setMaxRepsonseSize(uint32_t maxResponseSize) { this->_maxResponseSize = maxResponseSize; }
    void setTimeout(uint32_t timeout) { this->_timeout = timeout; }

    bool doGet(const std::string &uri, std::string &body);
    bool doGet(const std::string &uri, HttpHeader &header, std::string &body);
    bool doPost(const std::string &uri, const std::string &data, std::string &body);
    bool doPost(const std::string &uri, HttpHeader &header, const std::string &data, std::string &body);

private:
    static std::once_flag _globalSetup;

    CURL *_conn;
    CURLcode _error;

    // max response with curl
    uint32_t _maxResponseSize;
    uint32_t _timeout;
};

}
}
