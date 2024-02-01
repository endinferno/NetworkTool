#pragma once

#include <string>

#include <iconv.h>

#include "NonCopyable.hpp"

class FormatConvertor : public NonCopyable
{
public:
    explicit FormatConvertor(const std::string& fromFromat,
                             const std::string& toFormat);
    std::string Convert(const std::string& str);
    ~FormatConvertor();

private:
    iconv_t iconvFd_;
};
