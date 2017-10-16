//
// Created by Michael on 16/03/2017.
//
#include "utils/log.h"

#include "engine/engine.h"

namespace spider {
namespace engine {

void GlobalEngine::OnRequestComplete(http::WebSourceObject *web_source) {
    LogDebug("spider engine process web object %s, http_code %d", web_source->url().c_str(), web_source->GetHttpCode());

    // forwared to processor manager
    _processor->Add(web_source);
}

bool GlobalEngine::OnProcessComplete(http::WebSourceObject *webPage) {

    return false;
}

void GlobalEngine::OnRescheduleComplete(http::WebSourceObject *webPage) {

}

}
}
