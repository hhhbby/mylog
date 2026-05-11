#include <string>
using std::string;
#include <cctype>
#include <memory>

namespace mylog
{
class AppendFile {
public:
    AppendFile(std::string filename);
    ~AppendFile();
    void append(const char* data, size_t len);
    void flush();
    int64_t writtenBytes() const { return writtenBytes_; }
private:
    FILE* fp_;
    char buffer_[64*1024];  // 纯输出、靠指针/长度记录有效范围的底层缓冲区，分配即用，无需清零
    int64_t writtenBytes_;
};


class LogFile {
public:
    LogFile(string basename);
    void append(const char* data, size_t len);
    void flush();
    bool rollFile();
private:
    string getLogFileName(time_t *now); // 通过参数返回当前时间
    const string basename_;
    time_t startOfPeriod_;              // 上次滚动文件时那一天的起始时刻的秒数
    std::unique_ptr<AppendFile> file_;
};
} // namespace mylog
