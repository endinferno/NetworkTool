#include <array>

#include "SinaStockParser.hpp"

SinaStockParser::SinaStockParser()
    : convertor_("gb2312", "utf-8")
{}

Stock SinaStockParser::Parse(const std::string& msg)
{
    float tmp;
    std::array<char, 20> gbkName;
    float todayOpenPrice;
    float yesterdayOpenPrice;
    float latestTxnPrice;
    float highestTxnPrice;
    float lowestTxnPrice;
    double txnAmount;
    std::vector<float> bidPrice(5, 0.0);
    std::vector<float> askPrice(5, 0.0);

    Stock stock;
    std::sscanf(msg.data(),
                "var hq_str_%[^=]=\"%[^,],"
                "%f,%f,%f,%f,%f,%f,%f,%u,%lf,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%[^,],%[^,],%[^\"]\"",
                stock.id.data(),
                gbkName.data(),
                &todayOpenPrice,
                &yesterdayOpenPrice,
                &latestTxnPrice,
                &highestTxnPrice,
                &lowestTxnPrice,
                &tmp,
                &tmp,
                &stock.txnVolume,
                &txnAmount,
                &stock.bidVolume[0],
                &bidPrice[0],
                &stock.bidVolume[1],
                &bidPrice[1],
                &stock.bidVolume[2],
                &bidPrice[2],
                &stock.bidVolume[3],
                &bidPrice[3],
                &stock.bidVolume[4],
                &bidPrice[4],
                &stock.askVolume[0],
                &askPrice[0],
                &stock.askVolume[1],
                &askPrice[1],
                &stock.askVolume[2],
                &askPrice[2],
                &stock.askVolume[3],
                &askPrice[3],
                &stock.askVolume[4],
                &askPrice[4],
                stock.date.data(),
                stock.time.data(),
                stock.closeStatus.data());
    stock.todayOpenPrice = static_cast<uint32_t>(todayOpenPrice * 1000);
    stock.yesterdayOpenPrice = static_cast<uint32_t>(yesterdayOpenPrice * 1000);
    stock.latestTxnPrice = static_cast<uint32_t>(latestTxnPrice * 1000);
    stock.highestTxnPrice = static_cast<uint32_t>(highestTxnPrice * 1000);
    stock.lowestTxnPrice = static_cast<uint32_t>(lowestTxnPrice * 1000);
    stock.txnAmount = static_cast<uint64_t>(txnAmount * 1000);
    for (size_t i = 0; i < bidPrice.size(); i++) {
        stock.bidPrice[i] = static_cast<uint32_t>(bidPrice[i] * 1000);
        stock.askPrice[i] = static_cast<uint32_t>(askPrice[i] * 1000);
    }
    stock.name = convertor_.Convert(gbkName.data());
    return stock;
}
