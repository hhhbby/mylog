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
    Buffer& buffer();
private:
    Buffer buffer_;
};

} // namespace mylog
