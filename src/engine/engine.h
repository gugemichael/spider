//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <memory>

#include "common/web/web_object.h"
#include "scheduler/scheduler.h"
#include "spider/middleware/process_manager.h"

namespace spider {
namespace engine {

using namespace spider;

class GlobalEngine {
    DISALLOW_COPYING(GlobalEngine);

public:
    GlobalEngine() :
            _webQueue(4096),
            _processor(stdx::make_unique<processor::ProcessManager>()) {

    }

    void setScheduler(std::shared_ptr<scheduler::Scheduler> schd) {
        this->_scheduler = std::move(schd);
    }

    void process(url::WebObject* web);

private:
    std::shared_ptr<scheduler::Scheduler> _scheduler;

    // web content buffuered queue
    LazyNotifyQueue<url::WebObject*> _webQueue;
    // global processor
    std::unique_ptr<processor::ProcessManager> _processor;
};


}
}
