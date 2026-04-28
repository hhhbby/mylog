#include <iostream>
#include "LoggerManager.hpp"

int main()
{
    // std::cout<< "flushs make" << std::endl;
    // auto flushs = std::vector<std::shared_ptr<mylog::Flush>>{std::make_shared<mylog::ConsoleFlush>()};
    // std::cout<< "ptr make" << std::endl;
    // auto ptr = std::make_shared<mylog::Logger>
    //                 ("default", flushs);
    // std::cout<< "ptr end" << std::endl;
    // ptr->logDebug("hello");
    
    // mylog::getLoggerByName("mainLogger")->logDebug("debug");
    mylog::getLoggerByName("mainLogger")->logFatal("info");

    return 0;
}