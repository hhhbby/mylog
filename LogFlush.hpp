#pragma once

#include <iostream>
#include <fstream>
#include <memory>

namespace mylog {

class Flush {
public:
    virtual ~Flush() = default;
    virtual void flush(const char*, size_t) = 0;
};

class ConsoleFlush : public Flush {
public:
    void flush(const char* data, size_t len) override
    {
        // std::cout << "ConsoleFlush()" << std::endl;
        std::cout.write(data, len);
    }
};

class FileFlush : public Flush {
public:
    FileFlush(const std::string& filename) : fos_(filename, std::ios::app)  
    {
        if (!fos_.is_open())
        {
            std::cerr << "can not open file " << filename << std::endl;
            exit(-1);
        }
    }

    void flush(const char* data, size_t len) override
    {
        fos_.write(data, len);
    }

    ~FileFlush() override
    {
        fos_.flush();
        fos_.close();
    }

private:
    std::ofstream fos_;
};

// class FlushFactory {
// public:
//     template <typename FlushType, typename... Args>
//     static std::shared_ptr<FlushType> createFlush(Args &&... args)
//     {
//         return std::make_shared<FlushType>(std::forward<Args>(args)...));
//     }
// };

}