//
// Created by Michael on 10/9/16.
//

#pragma once

#include <list>
#include "engine/engine.h"

namespace spider {
namespace pipeline {

class Module {

};

class Pipeline {

public :
    void registerModule(std::shared_ptr<Module> module);

private :

    // managed modules
    std::list<std::shared_ptr<Module>> _modules;
    // global engine. not owned
    spider::engine::GlobalEngine *_engine;
};

}   // end of namespace spider
}   // end of namespace pipeline

