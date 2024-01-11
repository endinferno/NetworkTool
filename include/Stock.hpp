#pragma once

#include <string>
#include <vector>

class Stock
{
public:
    Stock()
        : id(8, '0')
        , name(20, '0')
        , bidVolume(5)
        , bidPrice(5)
        , askVolume(5)
        , askPrice(5)
        , date(10, '0')
        , time(8, '0')
        , closeStatus(2, '0')
    {}

    std::string id;
    std::string name;
    float todayOpenPrice;
    float yesterdayOpenPrice;
    float latestTxnPrice;
    float highestTxnPrice;
    float lowestTxnPrice;
    uint32_t txnVolume;
    float txnAmount;
    std::vector<uint32_t> bidVolume;
    std::vector<float> bidPrice;
    std::vector<uint32_t> askVolume;
    std::vector<float> askPrice;

    std::string date;
    std::string time;
    std::string closeStatus;
};
