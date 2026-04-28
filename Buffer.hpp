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
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};

}