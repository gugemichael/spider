//
// Created by Michael on 10/9/16.
//

#pragma once

#include "common/disallow_coping.h"
#include "common/web/web_object.h"

namespace spider {
namespace middleware {

using namespace spider;

class Processor {
    DISALLOW_COPYING(Processor);

public:
    Processor() = default;
    virtual ~Processor() = default;

public :
    virtual bool setup() = 0;
    virtual void handle(std::shared_ptr<url::WebObject> webObject) = 0;
};

}   // end of namespace middleware
}

