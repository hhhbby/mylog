#include "Logger.h"
#include "Worker.hpp"
#include "LogFile.h" 
#include <iostream>
#include <sys/time.h>

class Timestamp
{
public:
    explicit Timestamp(int64_t microSecondsSinceEpochArg)
        : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
    {
    }
    static Timestamp now()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int64_t seconds = tv.tv_sec;
        return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
    }

    int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
    static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
    int64_t microSecondsSinceEpoch_;
};

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

mylog::Worker* g_worker = nullptr;

void asyncOutput(const char* msg, size_t len)
{
  g_worker->push(msg, len);
}

std::unique_ptr<mylog::LogFile> g_logFile;

void outPutFunc(const char* msg, size_t len)
{
    g_logFile->append(msg, len);
}

void flushFunc()
{
    g_logFile->flush();
}

std::function<void(mylog::Buffer&)> f = [](mylog::Buffer& buf) {
    g_logFile->append(buf.begin(), buf.readableBytes());
};

void bench(bool )
{
    const int kBatch = 1024;
    string s(1024, 'c');


    Timestamp start = Timestamp::now();
    for (int i = 0; i < kBatch; ++i)
    {
      LOG_INFO << s;
    }
    Timestamp end =Timestamp::now();
    printf("1MiB log need time %f\n", timeDifference(end, start));

    // printf("timeDifference %f\n", timeDifference(end, start)*1000000/kBatch);
    // struct timespec ts = { 0, 500*1000*1000 };
    // nanosleep(&ts, NULL);
//   }

    s = string(1024*1024, 'c');
    start = Timestamp::now();
    for (int i = 0; i < kBatch; ++i)
    {
      LOG_INFO << s;
    }
    end = Timestamp::now();
    printf("1GiB log need time %f\n", timeDifference(end, start));
}

int main()
{
    g_logFile.reset(new mylog::LogFile("xxxproject"));
    mylog::Logger::setOutput(outPutFunc);
    mylog::Logger::setFlush(flushFunc);
    bench(false);
    printf("using async\n");

    mylog::Logger::setOutput(asyncOutput);
    mylog::Worker w(f);
    g_worker = &w;
    w.start();
    bench(false);
}