#pragma once

// #include "Worker.hpp"
// #include "LogFlush.hpp"
// #include "Formatter.hpp"
// #include <functional>
#include "LogStream.h"

namespace mylog
{
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    mylog::LogLevel g_logLevel = LogLevel::INFO;

    static const char* LogLevelName[static_cast<int>(LogLevel::NUM_LOG_LEVELS)] =
    {
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL",
    };

    void defaultOutput(const char* msg, int len)
    {
        size_t n = fwrite(msg, 1, len, stdout);
        //FIXME check n
        (void)n;
    }

    void defaultFlush()
    {
        fflush(stdout);
    }

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    OutputFunc g_output = defaultOutput;
    FlushFunc g_flush = defaultFlush;

    void setOutput(OutputFunc f)
    {
        g_output = f;
    }

class Logger {
public:
    Logger(const std::string& fileName, int line, LogLevel level)   // stream_ constructor    
    {
        time_t t_seconds = time(nullptr);   
        tm nowtime;
        localtime_r(&t_seconds, &nowtime);
        char buf[64] = {0};
        size_t len = strftime(buf, sizeof(buf), "%H:%M:%S", &nowtime);

        stream_ << "[" << fileName  << " : " << line  << "]" << "[" << string(buf, len)  << "]" 
                << "[" << string(LogLevelName[static_cast<int>(level)], 6)  << "]";
    }
    // void log(LogLevel level, const std::string& fileName, size_t line, const std::string payload)
    // {

    //     std::string msg = formatter.format();
    //     if (level == LogLevel::FATAL || level == LogLevel::ERROR)
    //     {
    //         /* Fixme:送给远程客户端*/
    //     }
    //     // worker_->push(msg.data(), msg.size());
    // }

    ~Logger()
    {
        stream_.buffer().append("\n", 1);
        const Buffer& buf(stream_.buffer());
        g_output(buf.begin(), buf.readableBytes());
        g_flush();
    }

    // void workerTodo(Buffer& buf)
    // {
    //     for (std::shared_ptr<Flush>& ptr : flushs_)
    //     {
    //         ptr->flush(buf.begin(), buf.readableBytes());
    //     }
    // }
    // std::string getLoggerName()
    // {
    //     return loggerName_;
    // }

    LogStream stream_;
    // std::shared_ptr<Worker> worker_;
};


} // namespace mylog

#define LOG_DEBUG if(mylog::g_logLevel <= mylog::LogLevel::DEBUG)\
    mylog::Logger(__FILE__, __LINE__, mylog::LogLevel::DEBUG).stream_
#define LOG_INFO if(mylog::g_logLevel <= mylog::LogLevel::INFO)\
    mylog::Logger(__FILE__, __LINE__, mylog::LogLevel::INFO).stream_
#define LOG_WARN mylog::Logger(__FILE__, __LINE__, mylog::LogLevel::WARN).stream_
#define LOG_ERROR mylog::Logger(__FILE__, __LINE__, mylog::LogLevel::ERROR).stream_
#define LOG_FATAL mylog::Logger(__FILE__, __LINE__, mylog::LogLevel::FATAL).stream_