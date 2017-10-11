//
// Created by Michael on 16/03/2017.
//

#include "engine.h"

namespace spider {
namespace engine {

void GlobalEngine::OnRequestComplete(url::WebPageObject *webPage) {
    LOG(INFO) << "spider engine process web object url:" << webPage->url() << ", httpcode:" << webPage->httpCode();

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
