//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <memory>
#include <vector>

#include "common/block_queue.h"

#include "processor.h"

namespace spider {
namespace processor {

class ProcessManager {

public :
    ProcessManager() :
            _webQueue(4096) {

    }

    void installMiddleware();

private :
    // register modules
    std::vector<std::shared_ptr<middleware::Processor>> _middlewares;

    spider::LazyNotifyQueue<url::WebObject*> _webQueue;
};

}
}

