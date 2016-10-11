//
// Created by Michael on 10/9/16.
//

#ifndef SPIDER_PIPELINE_H
#define SPIDER_PIPELINE_H

#include <list>

namespace spider {

namespace pipeline {

class Module {

};

class Pipeline {

public :
    void addModule(Module *module);

private :
    std::list<Module *> _modules;
};

}   // end of namespace spider

}   // end of namespace pipeline


#endif //SPIDER_PIPELINE_H
