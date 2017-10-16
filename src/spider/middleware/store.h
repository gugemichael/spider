//
// Created by Michael on 11/10/2017.
//

#ifndef SPIDER_STORE_H
#define SPIDER_STORE_H

#include "spider/middleware/processor.h"

namespace spider {
namespace middleware {

class Store : public Processor {
public:
    bool setup() override;

    void handle(http::WebSourceObject* web_source) override;
};

}
}

#endif //SPIDER_STORE_H
