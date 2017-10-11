//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <memory>
#include <vector>
#include <engine/engine.h>

#include "common/block_queue.h"

#include "processor.h"

namespace spider {
namespace processor {

class ProcessManager {
    typedef std::vector<std::shared_ptr<middleware::Processor>> MiddlewareList;

public :
    ProcessManager() :
            _webQueue(4096) {
    }

    void InstallAndPrepare();

private :
    // global engine
    engine::GlobalEngine *_engine;      // not owned

    // register modules
    MiddlewareList _middlewares;

    spider::LazyNotifyQueue<url::WebPageObject *> _webQueue;
};

}
}

