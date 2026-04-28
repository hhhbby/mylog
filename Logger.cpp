#include "Logger.h"

namespace mylog
{
    Logger::LogLevel initLogLevel()
    {
        return Logger::LogLevel::INFO;
    }

    Logger::LogLevel g_logLevel = initLogLevel();
    static const char* LogLevelName[static_cast<int>(Logger::LogLevel::NUM_LOG_LEVELS)] =
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
    Logger::OutputFunc g_output = defaultOutput;
    Logger::FlushFunc g_flush = defaultFlush;



    Logger::Logger(const std::string& fileName, int line, LogLevel level)
    {
        time_t t_seconds = time(nullptr);   
        tm nowtime;
        localtime_r(&t_seconds, &nowtime);
        char buf[64] = {0};
        size_t len = strftime(buf, sizeof(buf), "%H:%M:%S", &nowtime);

        stream_ << "[" << fileName  << " : " << line  << "]" << "[" << string(buf, len)  << "]" 
                << "[" << string(LogLevelName[static_cast<int>(level)], 6)  << "]";
    }
    Logger::~Logger()
    {
        stream_.buffer().append("\n", 1);
        const Buffer& buf(stream_.buffer());
        g_output(buf.begin(), buf.readableBytes());
        g_flush();
    }
    void Logger::setLogLevel(LogLevel level)
    {
        g_logLevel = level;
    }
    void Logger::setOutput(OutputFunc f)
    {
        g_output = f;
    }
    void Logger::setFlush(FlushFunc f)
    {
        g_flush = f;
    }

} // namespace mylog
