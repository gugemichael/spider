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
public:
    DISALLOW_COPYING(GlobalEngine);

    GlobalEngine() :
        _processor(stdx::make_unique<processor::ProcessManager>()) {

        _processor->InstallAndPrepare();
    }

    void UseScheduler(std::shared_ptr<scheduler::Scheduler> schd) {
        this->_scheduler = std::move(schd);
    }

    // invoke after Downloader finish
    void OnRequestComplete(http::WebSourceObject *web_source);

    // invoke after ProcessorManager handled
    //
    // return true. if the web object needs more processing
    bool OnProcessComplete(http::WebSourceObject *webPage);

    // invoke after Scheduler schedue()
    void OnRescheduleComplete(http::WebSourceObject *webPage);

    bool Startup() {
        auto run = _scheduler->RunAndJoin();
        _scheduler->Stop();

        return run;
    }

private:
    std::shared_ptr<scheduler::Scheduler> _scheduler;

    // global processor
    std::unique_ptr<processor::ProcessManager> _processor;
};


}
}
