// #include "Logger.h"
// #include "Worker.hpp"
// #include "LogFile.h" 
// #include <iostream>
// #include <sys/time.h>

// class Timestamp
// {
// public:
//     explicit Timestamp(int64_t microSecondsSinceEpochArg)
//         : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
//     {
//     }
//     static Timestamp now()
//     {
//         struct timeval tv;
//         gettimeofday(&tv, NULL);
//         int64_t seconds = tv.tv_sec;
//         return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
//     }

//     int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
//     static const int kMicroSecondsPerSecond = 1000 * 1000;

// private:
//     int64_t microSecondsSinceEpoch_;
// };

// inline double timeDifference(Timestamp high, Timestamp low)
// {
//     int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
//     return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
// }

// mylog::Worker* g_worker = nullptr;

// void asyncOutput(const char* msg, size_t len)
// {
//   g_worker->push(msg, len);
// }

// std::unique_ptr<mylog::LogFile> g_logFile;

// void outPutFunc(const char* msg, size_t len)
// {
//     g_logFile->append(msg, len);
// }

// void flushFunc()
// {
//     g_logFile->flush();
// }

// std::function<void(mylog::Buffer&)> f = [](mylog::Buffer& buf) {
//     g_logFile->append(buf.begin(), buf.readableBytes());
// };

// void bench(bool )
// {
//     const int kBatch = 1024 * 16;
//     string s(64, 'c');


//     Timestamp start = Timestamp::now();
//     for (int i = 0; i < kBatch; ++i)
//     {
//       LOG_INFO << s;
//     }
//     Timestamp end =Timestamp::now();
//     printf("1MiB log need time %f\n", timeDifference(end, start));

//     // printf("timeDifference %f\n", timeDifference(end, start)*1000000/kBatch);
//     // struct timespec ts = { 0, 500*1000*1000 };
//     // nanosleep(&ts, NULL);
// //   }

//     // s = string(1024*1024, 'c');
//     start = Timestamp::now();
//     const int kBatch2 = 1024 *1024 *16;
//     for (int i = 0; i < kBatch2; ++i)
//     {
//       LOG_INFO << s;
//     }
//     end = Timestamp::now();
//     printf("1GiB log need time %f\n", timeDifference(end, start));
// }

// int main()
// {
//     g_logFile.reset(new mylog::LogFile("xxxproject"));
//     mylog::Logger::setOutput(outPutFunc);
//     mylog::Logger::setFlush(flushFunc);
//     bench(false);
//     printf("using async\n");

//     mylog::Logger::setOutput(asyncOutput);
//     mylog::Worker w(f);
//     g_worker = &w;
//     w.start();
//     bench(false);
// }


#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include "Worker.hpp"
using mylog::Worker;
using mylog::Buffer;

void test_logger(Worker& worker, int num_threads, int msgs_per_thread) {
    std::vector<std::thread> threads;
    std::string msg = "This is a test log message, typical length.\n";
    
    // 1. 开始纯前端耗时计时
    auto start_time = std::chrono::high_resolution_clock::now();

    // 2. 瞬间启动多个线程疯狂写入
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < msgs_per_thread; ++j) {
                worker.push(msg.c_str(), msg.length());
            }
        });
    }

    // 3. 等待所有前端线程把数据塞入队列
    for (auto& t : threads) {
        t.join();
    }

    // 4. 停止计时（注意：此时后台 Worker 可能还在拼命写盘，不管它）
    auto end_time = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << num_threads << " 个线程共写入 " 
              << (num_threads * msgs_per_thread) << " 条日志。\n";
    std::cout << "前端 push 总耗时: " << duration.count() << " 毫秒\n";
    std::cout << "前端吞吐量: " 
              << (num_threads * msgs_per_thread) / (duration.count() / 1000.0) 
              << " 条/秒\n";
}

int main() {
    // 假设你的 callBack 里面是写文件或者简单丢弃
    Worker worker([](Buffer& ){ 
        // 模拟后台写盘，这里稍微 delay 一下，能更好地制造积压效果
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    });
    worker.start();

    // 测试：4个线程，每个写 25万条，共 100 万条
    test_logger(worker, 4, 2500000); 

    return 0;
    // main 结束时，worker 析构，才慢慢等后台刷完盘
}