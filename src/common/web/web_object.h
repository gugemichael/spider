//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <string>
#include <unordered_map>

#include "common/stdx/memory.h"

namespace spider {
namespace http {

enum HttpResponseCode {
    HTTP_OK = 200,
    HTTP_REDIRECTION = 300,
    HTTP_BAD_REQUEST = 400,
    HTTP_INTERNAL_ERROR = 500,
};

class WebSourceObject final {
public:
    using WebURL = std::string;
    using WebContent = std::string;
    using Outlink = std::unordered_map<std::string /*Url*/, uint32_t /*weight*/>;

public:
    explicit WebSourceObject(const WebURL& url, const WebContent& content) :
    _url(url),
    _raw_content(content),
    _http_code(0) {}

    ~WebSourceObject() = default;

    uint32_t GetHttpCode() const {
        return _http_code;
    }

    void SetHttpCode(uint32_t http_code) {
        _http_code = http_code;
    }

    const WebURL& url() const {
        return _url;
    }

    const WebContent& content() const {
        return _raw_content;
    }

    const Outlink& outlink() const {
        return _outlink;
    }

    void SetOutlink(Outlink& outlink) {
        _outlink = std::move(outlink);
    }

private:
    WebURL _url;
    WebContent _raw_content;

    // crawler response's http status code
    uint32_t _http_code;

    // outlinks : url's link weight
    Outlink _outlink;
};

}
}

