#pragma once

#include <array>

#include "StockParser.hpp"

class SinaStockParser : public StockParser
{
public:
    SinaStockParser() = default;
    Stock Parse(const std::string& msg) override;
    ~SinaStockParser() = default;

private:
    [[nodiscard]] std::string ConvertGbkToUtf8(
        const std::string& gbkName) const;

    std::array<char, 20> gbkName_;
};
