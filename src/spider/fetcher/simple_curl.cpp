//
// Created by Michael on 15/03/2017.
//

#include <functional>
#include <string>

#include "utils/log.h"
#include "utils/util.h"

#include "simple_curl.h"

HttpHeader emptyHeader;

namespace spider {
namespace fetcher {

std::once_flag SimpleHttpClient::_globalSetup;

bool SimpleHttpClient::Initail() {
    std::call_once(SimpleHttpClient::_globalSetup, [] {
        CURLcode code;
        if ((code = curl_global_init(CURL_GLOBAL_DEFAULT)) == CURLE_OK)
            LogInfo("curl global initialize successful");
        else
            LogError("curl global initialize failed : %d", code);
    });

    return (this->_conn = curl_easy_init()) != nullptr;
}

size_t request_handler(void *buffer, size_t size, size_t nmemb, void *user_p) {
    auto *destination = (std::string *) user_p;
    auto len = size * nmemb;
    destination->reserve(destination->size() + len + 4096);
    destination->append((const char *) buffer, size * nmemb);
    return size * nmemb;
}

CURLcode __curl_perform_and_clean(CURL *conn) {
    CURLcode code = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if (code != CURLE_OK)
        LogInfo("curl request perform failed : %s", curl_easy_strerror(code));

    return code;
}

bool SimpleHttpClient::RequestGet(const std::string& uri, std::string& body) {
    return this->RequestGet(uri, emptyHeader, body);
}


bool SimpleHttpClient::RequestGet(const std::string& uri, HttpHeader& header, std::string& body) {
    invariant(_conn != nullptr);

    CURLcode code = set_curl_common_opts(uri, body);
    if (code != CURLE_OK) {
        _error = code;
        curl_easy_cleanup(_conn);
        return false;
    }

    return (_error = __curl_perform_and_clean(_conn)) == CURLE_OK && (_conn = curl_easy_init());
}

bool SimpleHttpClient::RequestPost(const std::string& uri, const std::string& data, std::string& body) {
    return this->RequestPost(uri, emptyHeader, data, body);
}

bool SimpleHttpClient::RequestPost(const std::string& uri, HttpHeader& header, const std::string& data,
                                   std::string& body) {
    invariant(_conn != nullptr);

    char buffer[_maxResponseSize];
    if (set_curl_common_opts(uri, body) != CURLE_OK)
        return false;

    curl_easy_setopt(_conn, CURLOPT_POST, 1);
    curl_easy_setopt(_conn, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(_conn, CURLOPT_WRITEDATA, buffer);

    return (_error = __curl_perform_and_clean(_conn)) && (_conn = curl_easy_init());
}

CURLcode SimpleHttpClient::set_curl_common_opts(const std::string uri, std::string& resp) {
    CURLcode error;

    if ((error = curl_easy_setopt(_conn, CURLOPT_URL, uri.c_str())) != 0) {
        LogError("simple_curl CURLOPT_URL return error : %d", error);
        return error;
    }

    if ((error = curl_easy_setopt(_conn, CURLOPT_FOLLOWLOCATION, 1)) != 0) {
        curl_easy_setopt(_conn, CURLOPT_MAXREDIRS, 8);
        LogError("simple_curl CURLOPT_FOLLOWLOCATION return error : %d", error);
        return error;
    }

    if ((error = curl_easy_setopt(_conn, CURLOPT_TIMEOUT, _timeout)) != 0) {
        LogError("simple_curl CURLOPT_TIMEOUT return error : %d", error);
        return error;
    }

    if (_dumpUrlRequest)
        curl_easy_setopt(_conn, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(_conn, CURLOPT_WRITEDATA, &resp);
    curl_easy_setopt(_conn, CURLOPT_WRITEFUNCTION, request_handler);

    return CURLE_OK;
}

}
}
