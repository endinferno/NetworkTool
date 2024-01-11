#pragma once

#include <iconv.h>

#include "Stock.hpp"

class SinaStock : public Stock
{
public:
    SinaStock() = default;
    void Parse(const std::string& msg) override;
    ~SinaStock() = default;

private:
    [[nodiscard]] std::string ConvertGbkToUtf8(
        const std::string& gbkName) const;
};
