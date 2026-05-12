#pragma once
#include "Buffer.hpp"
#include <string>
using std::string;

namespace mylog
{
class LogStream {
    using self = LogStream;
public:
    self& operator<<(const string& str);
    self& operator<<(int n);
    const Buffer& buffer() const;
private:
    Buffer buffer_;
};

} // namespace mylog
