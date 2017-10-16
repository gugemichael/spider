//
// Created by Michael on 16/03/2017.
//

#include "spider/middleware/process_manager.h"
#include "spider/middleware/store.h"

namespace spider {
namespace processor {

void ProcessManager::InstallAndPrepare() {

    _middlewares.emplace_back(new middleware::Store);
}

bool ProcessManager::Iterate(http::WebSourceObject *web_source) {
    for (const auto& next : _middlewares) {
        next->handle(web_source);
    }

    return true;
}

void ProcessManager::Add(http::WebSourceObject *web_source) {
    _web_source_queue.offer(web_source);
}

}
}
