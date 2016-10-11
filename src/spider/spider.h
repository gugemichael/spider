//
// Created by Michael on 10/9/16.
//

#ifndef SPIDER_SPIDER_H
#define SPIDER_SPIDER_H

#include "pipeline/pipeline.h"

namespace spider {

using namespace std;
using namespace pipeline;

class Spider {

public :
    static string name() { return "spieder"; }

    Spider();
    Spider(Spider &spider) = delete;
    Spider(Spider &&spider) = delete;
    Spider(const Spider &spider) = delete;
    bool startupCrawl();

    // asdf
    bool a() = delete;

private :

    std::shared_ptr<Pipeline> _pipeline;
};

}


#endif //SPIDER_SPIDER_H


