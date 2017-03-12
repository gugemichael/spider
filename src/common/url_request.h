#pragma once

#include <string>

namespace url {

enum class HttpMethod {
    GET, POST
};

class DownloadRequest {

public:
    DownloadRequest(const std::string& uri) :
            _uri(uri), _httpMethod(HttpMethod::GET) {

    }
    ~DownloadRequest() = default;

private:
    std::string _uri;
    HttpMethod _httpMethod;
};


} // end namespace