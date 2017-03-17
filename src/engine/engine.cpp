//
// Created by Michael on 16/03/2017.
//

#include "engine.h"

namespace spider {
namespace engine {

void GlobalEngine::process(url::WebObject* web) {
    LOG(INFO) << "spider engine process web object url:" << web->url() << ", httpcode:" << web->httpCode();

    // forwared to processor manager
    this->_webQueue.offer(web);
}

}
}
