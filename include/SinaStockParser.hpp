#pragma once

#include "StockParser.hpp"
#include "Utils/FormatConvertor.hpp"

class SinaStockParser : public StockParser
{
public:
    SinaStockParser();
    Stock Parse(const std::string& msg) override;
    ~SinaStockParser() = default;

private:
    FormatConvertor convertor_;
};
