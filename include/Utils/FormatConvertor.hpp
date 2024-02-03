#pragma once

#include <iconv.h>

#include <string>

#include "NonCopyable.hpp"
#include "fmt/format.h"

class FormatConvertor : public NonCopyable
{
public:
    explicit FormatConvertor(const std::string& fromFromat,
                             const std::string& toFormat)
        : iconvFd_(::iconv_open(toFormat.c_str(), fromFromat.c_str()))
    {}
    std::string Convert(const std::string& str)
    {
        std::string outStr(str.size() * 2 + 1, '\0');
        char* inBuf = const_cast<char*>(str.c_str());
        size_t inLen = str.size();
        char* outBuf = const_cast<char*>(outStr.c_str());
        size_t outLen = outStr.size();
        size_t ret = ::iconv(iconvFd_, &inBuf, &inLen, &outBuf, &outLen);
        if (static_cast<int64_t>(ret) == -1) {
            throw std::runtime_error(
                fmt::format("Fail to convert string {}", str));
        }
        outStr.resize(outStr.size() - outLen);
        return outStr;
    }
    ~FormatConvertor()
    {
        ::iconv_close(iconvFd_);
    }

private:
    iconv_t iconvFd_;
};
