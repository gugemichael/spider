//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <memory>
#include <vector>

#include "common/block_queue.h"
#include "spider/middleware/processor.h"

namespace spider {
namespace processor {

namespace engine {
class GlobalEngine;
}

class ProcessManager {
    typedef std::vector<std::shared_ptr<middleware::Processor>> MiddlewareList;

public :
    ProcessManager() :
            _web_source_queue(4096) {
    }

    void InstallAndPrepare();

    void Add(http::WebSourceObject* web_source);

private :
    bool Iterate(http::WebSourceObject* web_source);

    // global engine
    engine::GlobalEngine *_engine;      // not owned

    // register modules
    MiddlewareList _middlewares;

    spider::LazyNotifyQueue<http::WebSourceObject *> _web_source_queue;
};

}
}

