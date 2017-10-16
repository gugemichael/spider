//
// Created by Michael on 18/03/2017.
//

#include <iostream>
#include <unordered_map>
#include <boost/regex.hpp>

#include "utils/log.h"

#include "spider/fetcher/web_parser.h"

namespace spider {
namespace fetcher {

static const boost::regex https_regex("http[s]?://.*?/");
static const boost::regex http_regex("http://.*?/");

bool WebParserImpl::parse(const std::string& web_content, http::WebSourceObject *web_source) {
    // key is url. Value is url's reference weight
    http::WebSourceObject::Outlink parsedUrls;
    boost::smatch found;

    auto next = web_content.begin();
    while (boost::regex_search(next, web_content.end(), found, http_regex)) {
        std::string url = found[0].str();
//        fprintf(stdout, "%s\n", found[0].str().c_str());
        if (parsedUrls.find(url) == parsedUrls.end())
            parsedUrls.insert(std::make_pair<std::string, uint32_t>(std::move(url), 0));
        else
            parsedUrls[url]++;
        next = found[0].second;
    }


    auto dedup = 0;
    std::for_each(parsedUrls.cbegin(), parsedUrls.cend(),
                  [&dedup](const std::pair<std::string, uint32_t>& url) { dedup += url.second; });

    web_source->SetHttpCode(http::HTTP_OK);
    web_source->SetOutlink(parsedUrls);

    LogInfo("web html regex parser extract %d urls from %s. deduplication %d", web_source->outlink().size(),
            web_source->url().c_str(), dedup);

    return true;
}

}
}
