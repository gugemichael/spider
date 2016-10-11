#include <iostream>
#include <vector>

#include "spider.h"

namespace spider {

bool Spider::startupCrawl() {
    std::cout << "start ... " << this->name() << std::endl;

    return true;
}

Spider::Spider() : _pipeline(std::make_shared<Pipeline>()) {
}


} // end of namespace

int main() {
    using namespace spider;

    Spider *spider = new Spider();
    spider->startupCrawl();

    delete spider;
    return 0;
}
