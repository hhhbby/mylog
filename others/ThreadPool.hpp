#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>

namespace mylog {
class ThreadPool
{
public:
    ThreadPool(size_t nums):stop(false)
    {
        workers.reserve(nums);
        while (nums--)
            workers.emplace_back(threadFunc, this);
    }

    template<typename F, typename... Args>
    void put(F &&f, Args&&... args)
    {
        std::unique_lock<std::mutex> lock(mutex);
        // 这里简单封装，生产环境建议用 std::future 获取返回值
        tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        cond.notify_one();  /* 有wait morphing*/
    }

    static void threadFunc(ThreadPool *arg)
    {
        std::function<void()> task;
        while (true)
        {
            {
                std::unique_lock<std::mutex> lk(arg->mutex);
                while (!arg->stop && arg->tasks.empty())
                    arg->cond.wait(lk);
                if (arg->stop && arg->tasks.empty())
                    return;
                task = std::move(arg->tasks.front());
                arg->tasks.pop();
            }
            task();
        }
    }

    ~ThreadPool() {
        {  
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        cond.notify_all(); // 唤醒所有线程来“领盒饭”准备退出
        for (std::thread &worker : workers) {
            if (worker.joinable()) 
            worker.join();
        }
    }


private:
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable cond;
    bool stop;
    /* 将worker放到最后*/
    std::vector<std::thread> workers;
};
}