#include "LogStream.h"

namespace mylog
{
    LogStream::self& LogStream::operator<<(const string& str)
    {
        buffer_.append(str.data(), str.size());
        return *this;
    }
    LogStream::self& LogStream::operator<<(int n)
    {
        string s = std::to_string(n);
        return *this << s;
    }

    Buffer& LogStream::buffer()
    {
        return buffer_;
    }
} // namespace mylog
