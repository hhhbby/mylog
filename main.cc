#include "Logger.hpp"
#include "Worker.hpp"
#include <iostream>

mylog::Worker* g_asyncLog = NULL;

void asyncOutput(const char* msg, int len)
{
  g_asyncLog->push(msg, len);
}
std::function<void(mylog::Buffer&)> f = [](mylog::Buffer& buf) {
    std::cout << "by other thread " << std::string(buf.begin(), buf.readableBytes());
};

int main()
{
    LOG_WARN << "xxx";
    mylog::setOutput(asyncOutput);
    mylog::Worker w(f);
    w.start();
    g_asyncLog = &w;
    LOG_WARN << "yyy";

}