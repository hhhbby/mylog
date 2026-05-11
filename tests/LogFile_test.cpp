#include "LogFile.h"
#include "Logger.h"

std::unique_ptr<mylog::LogFile> g_logFile;

void outPutFunc(const char* msg, size_t len)
{
    g_logFile->append(msg, len);
}

void flushFunc()
{
    g_logFile->flush();
}

int main()
{
    // fopen("filea", "a");
    LOG_INFO << "hello";
    g_logFile.reset(new mylog::LogFile("xxxproject"));
    mylog::Logger::setOutput(outPutFunc);
    mylog::Logger::setFlush(flushFunc);
    string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    for (int i = 0; i < 10000; ++i)
    {
        LOG_INFO << line << i;
    } 
}