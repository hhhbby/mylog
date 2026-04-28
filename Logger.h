#pragma once

#include "LogStream.h"

namespace mylog
{

class Logger {
public:
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    Logger(const std::string& fileName, int line, LogLevel level);
    ~Logger();
    LogStream& stream() { return stream_; }

    static inline LogLevel logLevel();
    static void setLogLevel(LogLevel);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    static  void setOutput(OutputFunc f);
    static  void setFlush(FlushFunc f);
    
private:
    LogStream stream_;
};  // class Logger end
    

// extern Logger::LogLevel g_logLevel = Logger::LogLevel::INFO;    // 这是定义！
extern Logger::LogLevel g_logLevel;
// 针对频繁调用的轻量级读取，牺牲封装性换取极致性能（Inline）；针对低频调用的写入或复杂逻辑，牺牲微小的调用开销换取更好的工程健壮性和编译速度。
inline Logger::LogLevel Logger::logLevel()  /*这里inline如果不加，会报错，multiple definition*/
{
    return g_logLevel;
}
// inline void Logger::setLogLevel(LogLevel level)
// {
//     g_logLevel = level;
// }
} // namespace mylog


/*
CAUTION: do not write:

if (good)
  LOG_INFO << "Good news";
else
  LOG_WARN << "Bad news";

this expends to

if (good)
  if (logging_INFO)
    logInfoStream << "Good news";
  else
    logWarnStream << "Bad news";

*/
#define LOG_DEBUG if(mylog::Logger::logLevel() <= mylog::Logger::LogLevel::DEBUG)\
    mylog::Logger(__FILE__, __LINE__, mylog::Logger::LogLevel::DEBUG).stream()
#define LOG_INFO if(mylog::Logger::logLevel() <= mylog::Logger::LogLevel::INFO)\
    mylog::Logger(__FILE__, __LINE__, mylog::Logger::LogLevel::INFO).stream()
#define LOG_WARN mylog::Logger(__FILE__, __LINE__, mylog::Logger::LogLevel::WARN).stream()
#define LOG_ERROR mylog::Logger(__FILE__, __LINE__, mylog::Logger::LogLevel::ERROR).stream()
#define LOG_FATAL mylog::Logger(__FILE__, __LINE__, mylog::Logger::LogLevel::FATAL).stream()
