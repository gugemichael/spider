//
// Created by Michael on 10/9/16.
//

#pragma once

#include <list>

namespace spider {
namespace pipeline {

class Module {

};

class ProcessorManager {

public :
    void registerModule(std::shared_ptr<Module> module);

private :

    // managed modules
    std::list<std::shared_ptr<Module>> _modules;
};

}   // end of namespace spider
}   // end of namespace pipeline

