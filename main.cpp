#include "Logger.h"
#include "Worker.hpp"
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

mylog::Worker* g_worker = NULL;

void asyncOutput(const char* msg, int len)
{
  g_worker->push(msg, len);
}
std::function<void(mylog::Buffer&)> f = [](mylog::Buffer& buf) {
    std::cout << "by other thread " << std::string(buf.begin(), buf.readableBytes());
};

void bench(bool longLog)
{

  int cnt = 0;
  const int kBatch = 1000;
  std::string empty = " ";
  std::string longStr(3000, 'X');
  longStr += " ";

//   for (int t = 0; t < 30; ++t)
//   {
    Timestamp start = Timestamp::now();
    for (int i = 0; i < kBatch; ++i)
    {
      LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
               << (longLog ? longStr : empty)
               << cnt;
      ++cnt;
    }
    Timestamp end =Timestamp::now();
    printf("timeDifference %f\n", timeDifference(end, start)*1000000/kBatch);
    struct timespec ts = { 0, 500*1000*1000 };
    nanosleep(&ts, NULL);
//   }
}

int main()
{
    LOG_WARN << "xxx";
    bench(false);
    mylog::Logger::setOutput(asyncOutput);
    mylog::Worker w(f);
    g_worker = &w;
    w.start();
    LOG_WARN << "yyy";
    bench(false);
    // bench(true);
}