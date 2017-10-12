//
// Created by Michael on 16/03/2017.
//
#include "utils/log.h"

#include "engine.h"

namespace spider {
namespace engine {

void GlobalEngine::OnRequestComplete(url::WebPageObject *webPage) {
    LogDebug("spider engine process web object %s, http_code %d", webPage->url().c_str(), webPage->httpCode());

    // forwared to processor manager
//    this->_webPagesQueue.offer(webPage);
//    _processorManager->InstallAndPrepare()
}

bool GlobalEngine::OnProcessComplete(url::WebPageObject *webPage) {

    return false;
}

void GlobalEngine::OnRescheduleComplete(url::WebPageObject *webPage) {

}

}
}
