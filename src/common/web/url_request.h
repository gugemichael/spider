#pragma once

#include <string>

namespace spider {
namespace url {

enum class HttpMethod {
    GET, POST
};

class DownloadRequest {

public:
    DownloadRequest(const std::string &uri) :
            _uri(uri), _httpMethod(HttpMethod::GET) {

    }
    ~DownloadRequest() = default;

    const std::string &uri() { return _uri; }
    HttpMethod &httpMethod() { return _httpMethod; }

private:
    std::string _uri;
    HttpMethod _httpMethod;
};

}
} // end namespace