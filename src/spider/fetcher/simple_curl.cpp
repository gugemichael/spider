//
// Created by Michael on 15/03/2017.
//

#include <functional>
#include <glog/logging.h>

#include "common/basic.h"

#include "simple_curl.h"

HttpHeader emptyHeader;

namespace spider {
namespace fetcher {

std::once_flag SimpleCUrl::_globalSetup;

bool SimpleCUrl::initialize() {
    std::call_once(SimpleCUrl::_globalSetup, [] {
        CURLcode code;
        if ((code = curl_global_init(CURL_GLOBAL_DEFAULT)) == CURLE_OK)
            LOG(INFO) << "curl global initialize successful";
        else
            LOG(ERROR) << "curl global initialize failed : " << code;
    });

    return (this->_conn = curl_easy_init()) != nullptr;
}

size_t request_handler(void *buffer, size_t size, size_t nmemb, void *user_p) {
    std::string *destination = (std::string *) user_p;
    destination->assign((const char *) buffer, size * nmemb);
    return size * nmemb;
}

CURLcode __curl_common_opt(CURL *conn, const std::string uri, uint32_t timeout, std::string &body) {
    CURLcode error;

    if ((error = curl_easy_setopt(conn, CURLOPT_URL, uri.c_str())) != 0) {
        LOG(ERROR) << "simple_curl CURLOPT_URL return error : " << error;
        return error;
    }

    if ((error = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1)) != 0) {
        curl_easy_setopt(conn, CURLOPT_MAXREDIRS, 8);
        LOG(ERROR) << "simple_curl CURLOPT_FOLLOWLOCATION return error : " << error;
        return error;
    }

    if ((error = curl_easy_setopt(conn, CURLOPT_TIMEOUT, timeout)) != 0) {
        LOG(ERROR) << "simple_curl CURLOPT_TIMEOUT return error : " << error;
        return error;
    }

    curl_easy_setopt(conn, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(conn, CURLOPT_WRITEDATA, &body);
    curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, request_handler);

    return CURLE_OK;
}

bool __curl_perform_and_clean(CURL *conn) {
    CURLcode error = curl_easy_perform(conn);
    curl_easy_cleanup(conn);

    if (error != CURLE_OK)
        LOG(WARNING) << "curl request perform failed : " << curl_easy_strerror(error);

    return CURLE_OK;
}

bool SimpleCUrl::doGet(const std::string &uri, std::string &body) {
    return this->doGet(uri, emptyHeader, body);
}


bool SimpleCUrl::doGet(const std::string &uri, HttpHeader &header, std::string &body) {
    invariant(_conn != nullptr);

    if (__curl_common_opt(_conn, uri, _timeout, body) != CURLE_OK) {
        curl_easy_cleanup(_conn);
        return false;
    }

    return __curl_perform_and_clean(_conn) && (_conn = curl_easy_init());
}

bool SimpleCUrl::doPost(const std::string &uri, const std::string &data, std::string &body) {
    return this->doPost(uri, emptyHeader, data, body);
}

bool SimpleCUrl::doPost(const std::string &uri, HttpHeader &header, const std::string &data, std::string &body) {
    invariant(_conn != nullptr);

    char buffer[_maxResponseSize];
    if (__curl_common_opt(_conn, uri, _timeout, body) != CURLE_OK)
        return false;

    curl_easy_setopt(_conn, CURLOPT_POST, 1);
    curl_easy_setopt(_conn, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(_conn, CURLOPT_WRITEDATA, buffer);

    return __curl_perform_and_clean(_conn) && (_conn = curl_easy_init());
}

}
}
