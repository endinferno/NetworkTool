#include <array>

#include "SinaStockParser.hpp"

SinaStockParser::SinaStockParser()
    : convertor_("gb2312", "utf-8")
{}

Stock SinaStockParser::Parse(const std::string& msg)
{
    float tmp;
    std::array<char, 20> gbkName;
    Stock stock;
    std::sscanf(msg.data(),
                "var hq_str_%[^=]="
                "\"%[^,],%f,%f,%f,%f,%f,%f,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%[^,],%[^,],%[^\"]\"",
                stock.id.data(),
                gbkName.data(),
                &stock.todayOpenPrice,
                &stock.yesterdayOpenPrice,
                &stock.latestTxnPrice,
                &stock.highestTxnPrice,
                &stock.lowestTxnPrice,
                &tmp,
                &tmp,
                &stock.bidVolume[0],
                &stock.bidPrice[0],
                &stock.bidVolume[1],
                &stock.bidPrice[1],
                &stock.bidVolume[2],
                &stock.bidPrice[2],
                &stock.bidVolume[3],
                &stock.bidPrice[3],
                &stock.bidVolume[4],
                &stock.bidPrice[4],
                &stock.askVolume[0],
                &stock.askPrice[0],
                &stock.askVolume[1],
                &stock.askPrice[1],
                &stock.askVolume[2],
                &stock.askPrice[2],
                &stock.askVolume[3],
                &stock.askPrice[3],
                &stock.askVolume[4],
                &stock.askPrice[4],
                stock.date.data(),
                stock.time.data(),
                stock.closeStatus.data());
    stock.name = convertor_.Convert(gbkName.data());
    return stock;
}
