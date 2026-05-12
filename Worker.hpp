#pragma once

#include "Buffer.hpp"
#include <condition_variable>
#include <functional>
#include <thread>
#include <atomic>

#include <unistd.h>

namespace mylog
{
class Worker {
public:

    Worker(const std::function<void(Buffer&)>& callBack)
            :callBack_(callBack),
            stop_(false)
            { }

    void start()    /*避免在构造函数的初始化列表中启动线程*/
    {
        if (!worker_.joinable())
            worker_ = std::thread(&Worker::threadFunc, this);
    }
    ~Worker()
    {
        stop();
    }

    /* 存放日志内容进入*/
    void push(const char* data, size_t len)
    {
        std::lock_guard<std::mutex> lk(mutex_);
        // 减少系统调用次数
        bool wasEmpty = (producerBuf_.readableBytes() == 0);
        producerBuf_.append(data, len);
        if (wasEmpty)
            cond_.notify_one();
    }


private:
    void stop()
    {
        // sleep(1);
        {
            std::lock_guard<std::mutex> lk(mutex_);
            stop_=true;
        }
        // stop_=true;
        cond_.notify_all();
        // std::cout << "back of notify"<< std::endl;
        // std::cout << "wait thread worker end" << std::endl;
        if (worker_.joinable())
            worker_.join();
        // std::cout << "thread worker is end" << std::endl;
    }
    void threadFunc()
    {
        while(true)
        {
            {
                // std::cout << "work thread id " << std::this_thread::get_id() << std::endl;
                std::unique_lock<std::mutex> lk(mutex_);
                // std::cout << "1.producerBuf_.readableBytes() is " << producerBuf_.readableBytes() << std::endl;
                while( !(producerBuf_.readableBytes() > 0) && !stop_)
                {
                    // sleep(2);
                    // std::cout << "front of wait"<< std::endl;
                    cond_.wait(lk);
                }
                // std::cout << "2.producerBuf_.readableBytes() is " << producerBuf_.readableBytes() << std::endl;
                if (stop_ && producerBuf_.readableBytes() == 0)
                    return;
                consumerBuf_.swap(producerBuf_);
            }
            callBack_(consumerBuf_);
            consumerBuf_.reset();
        }
    }


private:
    Buffer producerBuf_;
    Buffer consumerBuf_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::function<void(Buffer&)> callBack_;
    bool stop_;

    std::thread worker_;

};

} // namespace mylog
