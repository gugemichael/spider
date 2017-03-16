//
// Created by Michael on 16/03/2017.
//

#pragma once

class ProcessManager {

public :
    void installMiddleware(std::shared_ptr<Processor> module);

private :

    // managed modules
    std::list<std::shared_ptr<Processor>> _modules;
};


