//
// Created by Michael on 11/10/2017.
//

#ifndef SPIDER_STORAGE_H
#define SPIDER_STORAGE_H

#include "spider/middleware/processor.h"

namespace spider {

class Storage {
public:
    Storage() = default;
    virtual ~Storage() = default;

    virtual bool init() = 0;


};

}


#endif //SPIDER_STORAGE_H
