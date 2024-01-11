#pragma once

#include "Stock.hpp"

class StockParser
{
public:
    StockParser() = default;
    virtual Stock Parse(const std::string& msg) = 0;
    ~StockParser() = default;
};
