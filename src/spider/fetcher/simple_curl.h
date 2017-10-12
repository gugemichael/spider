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

class SimpleHttpClient {
public:
    DISALLOW_COPYING(SimpleHttpClient);

    SimpleHttpClient() :
            _maxResponseSize(DEFAULT_MAX_RESPONSE_SIZE),
            _conn(nullptr),
            _error(CURLE_OK),
            _dumpUrlRequest(false),
            _timeout(30) {}

    ~SimpleHttpClient() = default;

    CURLcode set_curl_common_opts(std::string uri, std::string& resp);

    bool Initail();
    bool IsError() { return _error != 0; }

    const char *GetLastError() { return curl_easy_strerror(_error); }

    void SetMaxRepsonseSize(uint32_t maxResponseSize) { this->_maxResponseSize = maxResponseSize; }
    void SetTimeout(uint32_t timeout) { this->_timeout = timeout; }

    bool RequestGet(const std::string& uri, std::string& body);
    bool RequestGet(const std::string& uri, HttpHeader& header, std::string& body);
    bool RequestPost(const std::string& uri, const std::string& data, std::string& body);
    bool RequestPost(const std::string& uri, HttpHeader& header, const std::string& data, std::string& body);

private:
    static std::once_flag _globalSetup;

    CURL* _conn;
    CURLcode _error;

    // max response with curl
    uint32_t _maxResponseSize;
    uint32_t _timeout;

    bool _dumpUrlRequest;
};

}
}
