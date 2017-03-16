//
// Created by Michael on 16/03/2017.
//

#pragma once

#include <memory>

#include "scheduler/scheduler.h"

namespace spider {
namespace engine {

using namespace spider::scheduler;

class GlobalEngine {

public:
    void setScheduler(std::shared_ptr<scheduler::Scheduler> schd) {
        this->_scheduler = std::move(schd);
    }

    void process(spider::fetcher::DownloadResponse *response);

private:
    std::shared_ptr<scheduler::Scheduler> _scheduler;
};


}
}
