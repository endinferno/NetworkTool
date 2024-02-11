#pragma once

#include <string>
#include <vector>

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
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
    uint32_t todayOpenPrice;
    uint32_t yesterdayOpenPrice;
    uint32_t latestTxnPrice;
    uint32_t highestTxnPrice;
    uint32_t lowestTxnPrice;
    uint32_t txnVolume;
    uint64_t txnAmount;
    std::vector<uint32_t> bidVolume;
    std::vector<uint32_t> bidPrice;
    std::vector<uint32_t> askVolume;
    std::vector<uint32_t> askPrice;

    std::string date;
    std::string time;
    std::string closeStatus;
};
// NOLINTEND(misc-non-private-member-variables-in-classes)
