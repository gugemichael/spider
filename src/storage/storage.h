//
// Created by Michael on 10/17/16.
//

#pragma once

namespace spider {

namespace storage {

class Storage {

public :
    Storage(Storage &storage) = delete;
    Storage(Storage &&storage) = delete;

    virtual bool create() = 0;
};

}

}
