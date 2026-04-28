#pragma once

#include <vector>
#include <algorithm>

namespace mylog{


class Buffer {
public:

    static const size_t kInitialSize = 256;

    Buffer()
      : readerIndex_(0),
        writerIndex_(0),
        buffer_(kInitialSize) {
            // std::cout << "Buffer()" << std::endl;

        }

    void append(const char *data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data+len, buffer_.begin() + writerIndex_);
        writerIndex_ += len;
    }

    void append(int x)  // // Efficient Integer to String Conversions, by Matthew Wilson.
    {
        static const char digits[20] = "9876543210123456789";     // for append(int)
        const char *zero = digits + 9;
        char buf[11] = {0};     // int带上符号最多有 -2147482648 共11位
        char* p = buf;
        int  i = x;
        do
        {
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);
        if (x < 0)
            *p++ = '-';
        std::reverse(buf, p);
        append(buf, p - buf);
    }

    void swap(Buffer &rhs)
    {
        buffer_.swap(rhs.buffer_);
        std::swap(readerIndex_, rhs.readerIndex_);
        std::swap(writerIndex_, rhs.writerIndex_);
    }

    const char *begin() const
    {
        return buffer_.data();
    }

    size_t writableBytes()  const
    {
        return buffer_.size() - writerIndex_;
    }

    size_t readableBytes()  const
    {
        return writerIndex_ - readerIndex_;
    }

    void reset()
    {
        readerIndex_ = 0;
        writerIndex_ = 0;
    }

protected:
    void ensureWritableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            // std::cout  << "size is " << writerIndex_+len << std::endl;
            buffer_.resize(writerIndex_+len);
        }
    }

private:
    size_t readerIndex_;
    size_t writerIndex_;
    std::vector<char> buffer_;
};

}