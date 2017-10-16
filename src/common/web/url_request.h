#pragma once

#include <string>
#include <utility>

namespace spider {
namespace http {

enum class HttpMethod {
    GET, POST
};

class DownloadRequest {

public:
    explicit DownloadRequest(std::string uri) :
        _uri(std::move(uri)),
        _httpMethod(HttpMethod::GET) {}

    ~DownloadRequest() = default;

    const std::string& uri() { return _uri; }

    const HttpMethod& httpMethod() { return _httpMethod; }

private:
    std::string _uri;
    HttpMethod _httpMethod;
};

}
} // end namespace