#pragma once

namespace mylog{

enum class LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
};

static const char* LogLevelName[static_cast<int>(LogLevel::NUM_LOG_LEVELS)] =
{
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL",
};

}