//
// Created by Michael on 18/03/2017.
//

#pragma once

#include "common/web/web_object.h"

namespace spider {
namespace fetcher {

enum WebParserHandle {
    HEADER_PARSER = 0,
    PAGE_CONTENT_PARSER,

    ALL_PARSER_COUNT
};

enum class ContentType {
    APPLICATION_JSON, APPLICATION_TEXT, TEXT_XML, TEXT_PLAIN
};

using namespace spider;

/**
 * Parser interface. include concrete header、content parser
 *
 */
class WebParser {

public:
    WebParser() = default;
    virtual ~WebParser() = default;

    virtual bool parse(url::WebObject* webObject) = 0;
};

class PageContentParser final : public WebParser {

public :
    PageContentParser() = default;
    virtual ~PageContentParser() = default;

    virtual bool parse(url::WebObject* webObject) override;
};

class HeaderParser final : public WebParser {

public :
    HeaderParser() = default;
    virtual ~HeaderParser() = default;

    virtual bool parse(url::WebObject* webObject) override;
};


}
}
