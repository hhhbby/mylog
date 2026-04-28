#include "Logger.hpp"
#include <unordered_map>
// #include <

namespace mylog {
class LoggerManager {
public:
    static LoggerManager& getinstance()
    {
        static LoggerManager singleton;
        return singleton;
    }

    void addLogger(const std::shared_ptr<Logger> &loggerPtr)
    {
        std::string loggerName = loggerPtr->getLoggerName();
        auto it = loggers_.find(loggerName);
        if (it != loggers_.end())
            return;
        loggers_.insert(std::make_pair(loggerName, loggerPtr));
    }
    
    std::shared_ptr<Logger> getLogger(const std::string& loggerName)
    {
        auto it = loggers_.find(loggerName);
        if (it != loggers_.end())
            return it->second;
        return defaultLogger_;        
    }

private:
    LoggerManager()
    {
        // std::cout << "LoggerManager()" << std::endl;
        defaultLogger_ = std::make_shared<Logger>
                    ("default", std::vector<std::shared_ptr<Flush>>{std::make_shared<ConsoleFlush>()});
        // std::cout << "the end of LoggerManager()" << std::endl;
        loggers_["default"] = defaultLogger_;
    }

    // std::mutex mutex_;
    std::shared_ptr<Logger> defaultLogger_;
    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;  // logger name to ptr
};


// 用户获取日志器
std::shared_ptr<Logger> getLoggerByName(const std::string &name) 
{
    return LoggerManager::getinstance().getLogger(name);
}
// 用户获取默认日志器
std::shared_ptr<Logger> DefaultLogger() 
{ 
    return LoggerManager::getinstance().getLogger("default");
}

LogLevel g_logLevel = LogLevel::INFO;

void setLogLevel(LogLevel level)
{
  g_logLevel = level;
}

}


#define logDebug(msg) if(mylog::g_logLevel <= mylog::LogLevel::DEBUG)\
    mylog::getLoggerByName("mainLogger")->log(mylog::LogLevel::DEBUG,  __FILE__, __LINE__, msg);
#define logInfo(msg) if(mylog::g_logLevel <= mylog::LogLevel::INFO)\
    log(mylog::LogLevel::INFO,  __FILE__, __LINE__, msg);
#define logWarn(msg) log(mylog::LogLevel::WARN,  __FILE__, __LINE__, msg);
#define logError(msg) log(mylog::LogLevel::ERROR,  __FILE__, __LINE__, msg);
#define logFatal(msg) log(mylog::LogLevel::FATAL,  __FILE__, __LINE__, msg);
