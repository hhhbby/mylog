#pragma once

#include "Worker.hpp"
#include "LogFlush.hpp"
#include "Formatter.hpp"
#include <functional>

namespace mylog
{
class Logger {
public:
    Logger(const std::string& name, const std::vector<std::shared_ptr<Flush>> &flushs)
            :loggerName_(name),
            flushs_(flushs.begin(), flushs.end()),
            worker_(std::make_shared<Worker>(std::bind(&Logger::workerTodo, this, std::placeholders::_1)))
            { 
                worker_->start();
                // std::cout << "Logger()" << std::endl;
            }

    void log(LogLevel level, const std::string& fileName, size_t line, const std::string payload)
    {
        Formatter formatter(level, fileName, line, loggerName_, payload);
        std::string msg = formatter.format();
        if (level == LogLevel::FATAL || level == LogLevel::ERROR)
        {
            /* Fixme:送给远程客户端*/
        }
        worker_->push(msg.data(), msg.size());
    }

    std::string getLoggerName()
    {
        return loggerName_;
    }

    void workerTodo(Buffer& buf)
    {
        for (std::shared_ptr<Flush>& ptr : flushs_)
        {
            ptr->flush(buf.begin(), buf.readableBytes());
        }
    }
private:
    std::string loggerName_;

    std::vector<std::shared_ptr<Flush>> flushs_;
    std::shared_ptr<Worker> worker_;
};
} // namespace mylog
