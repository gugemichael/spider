//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <string>
#include <unordered_map>

namespace spider {
namespace url {

enum HttpStatusCode {
    STATUS_OK = 200,
    STATUS_REDIRECTION = 300,
    STATUS_BAD_REQUEST = 400,
    STATUS_INTERNAL_ERROR = 500,
};

class WebObject final {
    using WebURL = std::string;
    using WebContent = std::string;
    using WebHeader = std::unordered_map<std::string, std::string>;

public:
    WebObject(WebURL url) : _url(url) {}
    ~WebObject() = default;

    uint32_t httpCode() const {
        return _httpCode;
    }
    const WebHeader& headerMap() const {
        return _headerMap;
    }
    const WebURL& url() const {
        return _url;
    }
    const WebContent& rawContent() const {
        return _rawContent;
    }

    bool resovleContent();

private:
    std::unique_ptr<WebObject> _source;

    // crawler response's http status code
    uint32_t _httpCode;

    WebHeader _headerMap;

    WebURL _url;
    WebContent _rawContent;
};

}
}

