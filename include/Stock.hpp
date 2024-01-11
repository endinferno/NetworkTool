#pragma once

#include <string>
#include <vector>

class Stock
{
public:
    Stock()
        : id_(6, '0')
        , name_(20, '0')
        , bidVolume_(5)
        , bidPrice_(5)
        , askVolume_(5)
        , askPrice_(5)
        , date_(10, '0')
        , time_(8, '0')
        , closeStatus_(2, '0')
    {}
    virtual void Parse(const std::string& msg) = 0;

    [[nodiscard]] std::string GetId() const
    {
        return id_;
    }
    [[nodiscard]] std::string GetName() const
    {
        return name_;
    }
    [[nodiscard]] float GetTodayOpenPrice() const
    {
        return todayOpenPrice_;
    }
    [[nodiscard]] float GetYesterdayOpenPrice() const
    {
        return yesterdayOpenPrice_;
    }
    [[nodiscard]] float GetLatestTxnPrice() const
    {
        return latestTxnPrice_;
    }
    [[nodiscard]] float GetHighestTxnPrice() const
    {
        return highestTxnPrice_;
    }
    [[nodiscard]] float GetLowestTxnPrice() const
    {
        return lowestTxnPrice_;
    }
    [[nodiscard]] uint32_t GetTxnVolume() const
    {
        return txnVolume_;
    }
    [[nodiscard]] float GetTxnAmount() const
    {
        return txnAmount_;
    }
    [[nodiscard]] uint32_t GetBidVolume(size_t idx = 0) const
    {
        return bidVolume_[idx];
    }
    [[nodiscard]] float GetBidPrice(size_t idx = 0) const
    {
        return bidPrice_[idx];
    }
    [[nodiscard]] uint32_t GetAskVolume(size_t idx = 0) const
    {
        return askVolume_[idx];
    }
    [[nodiscard]] float GetAskPrice(size_t idx = 0) const
    {
        return askPrice_[idx];
    }
    [[nodiscard]] std::string GetDate() const
    {
        return date_;
    }
    [[nodiscard]] std::string GetTime() const
    {
        return time_;
    }
    [[nodiscard]] std::string GetCloseStatus() const
    {
        return closeStatus_;
    }
    ~Stock() = default;

protected:
    std::string id_;
    std::string name_;
    float todayOpenPrice_;
    float yesterdayOpenPrice_;
    float latestTxnPrice_;
    float highestTxnPrice_;
    float lowestTxnPrice_;
    uint32_t txnVolume_;
    float txnAmount_;
    std::vector<uint32_t> bidVolume_;
    std::vector<float> bidPrice_;
    std::vector<uint32_t> askVolume_;
    std::vector<float> askPrice_;

    std::string date_;
    std::string time_;
    std::string closeStatus_;
};
