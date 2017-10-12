//
// Created by Michael on 18/03/2017.
//

#pragma once

#include "common/web/web_object.h"

namespace spider {
namespace fetcher {

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

    virtual bool parse(const std::string& web_content, url::WebPageObject *web_object) = 0;
};

class WebParserImpl final : public WebParser {
public :
    WebParserImpl() = default;

    ~WebParserImpl() override = default;

    bool parse(const std::string& web_content, url::WebPageObject *web_object) override;
};


}
}
