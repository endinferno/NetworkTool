#pragma once

#include "FormatConvertor.hpp"
#include "StockParser.hpp"

class SinaStockParser : public StockParser
{
public:
    SinaStockParser();
    Stock Parse(const std::string& msg) override;
    ~SinaStockParser() = default;

private:
    FormatConvertor convertor_;
};
