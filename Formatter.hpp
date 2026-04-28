#pragma once

#include <string>
#include <sstream>
#include "LogLevel.hpp"

#include <ctime>
#include <thread>


namespace mylog 
{
    class Formatter
    {
    public:
        Formatter(LogLevel level, std::string fileName, size_t line, std::string name, std::string payload)
               :level_(level), fileName_(fileName), line_(line),
               name_(name), payload_(payload), tid_(std::this_thread::get_id()) { }

        std::string format()
        {
            /* 预留空间，减少分配*/
            std::string ret;
            ret.reserve(256);

            /* 获取当前时间戳，并转换为tm结构体，方便解析*/
            time_t t_seconds = time(nullptr);   
            tm nowtime;
            localtime_r(&t_seconds, &nowtime);


            std::stringstream ss_tid;
            ss_tid << tid_;

            char buf[64] = {0};
            size_t len = strftime(buf, sizeof(buf), "%H:%M:%S", &nowtime);

            ret.append("[").append(buf, len).append("][");
            ret.append(LogLevelName[static_cast<int>(level_)]).append("][");
            ret.append(name_).append("][");
            ret.append(ss_tid.str()).append("][");
            ret.append(fileName_).append(":").append(std::to_string(line_));
            ret.append("] ").append(payload_).append("\n");

            return ret;
        }
    private:
        LogLevel level_; // 等级
        std::string fileName_; // 文件名
        size_t line_;           // 行号
        std::string name_;      // 日志器名
        std::string payload_;   // 信息体
        std::thread::id tid_;   // 线程id
    };
}
