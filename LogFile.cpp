#include "LogFile.h"
#include <cassert>
#include <cstring>  // for strerror

namespace mylog
{

AppendFile::AppendFile(std::string filename)
  : fp_(fopen(filename.c_str(), "ae")), 
    writtenBytes_(0)
{
    assert(fp_);
    ::setbuffer(fp_, buffer_, sizeof buffer_);  // 系统默认的用户缓冲区不够大，减少系统调用（提升性能）
}

AppendFile::~AppendFile()
{
    ::fclose(fp_);
}

/* 追加到文件尾*/
void AppendFile::append(const char* data, size_t len)
{
    size_t written = 0;
    while (written != len)
    {
        int n = ::fwrite_unlocked(data + written, 1, len - written, fp_);
        if (n != len - written) {
            if (::ferror(fp_))
            {
                fprintf(stderr, "AppendFile::append() failed %s\n", strerror(errno)); // strerror接受的是系统错误码，这里未考虑多线程
                break;
            }
        }
        written += n;
    }
    writtenBytes_ += written;
}

void AppendFile::flush()
{
    ::fflush(fp_);
}

// LogFile
LogFile::LogFile(string basename) : basename_(basename), startOfPeriod_(0)
{
    rollFile();
}


void LogFile::append(const char* data, size_t len)
{
    file_->append(data, len);
}

void LogFile::flush()
{
    file_->flush();
}

bool LogFile::rollFile()
{
    time_t now = 0;
    string filename = getLogFileName(&now);
    time_t start = now / (60*60*24) * (60*60*24);
    if (start > startOfPeriod_)
    {
        startOfPeriod_ = start;
        file_.reset(new AppendFile(filename));
        return true;
    }
    return false;
}

string LogFile::getLogFileName(time_t *now)
{
    string filename;
    filename.reserve(basename_.size() + 64);
    filename = basename_;

    time(now);
    struct tm tm;
    localtime_r(now, &tm);
    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S", &tm);
    filename += timebuf;
    filename += ".log";
    return filename;
}



} // namespace mylog
