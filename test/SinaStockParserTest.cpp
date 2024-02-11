#include "SinaStockParser.hpp"

#include "gtest/gtest.h"

TEST(SinaStockParserTest, SinaStockParserTest)
{
    std::string msg = "var hq_str_sz002603=\"";
    msg.push_back(static_cast<char>(0xd2));
    msg.push_back(static_cast<char>(0xd4));
    msg.push_back(static_cast<char>(0xc1));
    msg.push_back(static_cast<char>(0xeb));
    msg.push_back(static_cast<char>(0xd2));
    msg.push_back(static_cast<char>(0xa9));
    msg.push_back(static_cast<char>(0xd2));
    msg.push_back(static_cast<char>(0xb5));
    msg.append(",21.700,21.910,21.050,21.700,21.040,21.050,21.060,11715185,"
               "250256467.350,67622,21.050,29500,21.040,25400,21.030,38100,21."
               "020,46900,21.010,14000,21.060,13000,21.070,4600,21.080,20400,"
               "21.090,23600,21.100,2024-01-30,15:00:00,00\";");

    SinaStockParser parser;
    Stock stock = parser.Parse(msg);

    EXPECT_EQ(stock.id, "sz002603");
    EXPECT_EQ(stock.name, "以岭药业");
    EXPECT_EQ(stock.todayOpenPrice, 21700);
    EXPECT_EQ(stock.yesterdayOpenPrice, 21910);

    EXPECT_EQ(stock.latestTxnPrice, 21050);
    EXPECT_EQ(stock.highestTxnPrice, 21700);
    EXPECT_EQ(stock.lowestTxnPrice, 21040);
    EXPECT_EQ(stock.txnVolume, 11715185);
    EXPECT_EQ(stock.txnAmount, 250256467350);
    EXPECT_EQ(stock.bidVolume.size(), 5);
    EXPECT_EQ(stock.bidPrice.size(), 5);
    EXPECT_EQ(stock.askVolume.size(), 5);
    EXPECT_EQ(stock.askPrice.size(), 5);
    std::vector<uint32_t> bidVolumeAns = { 67622, 29500, 25400, 38100, 46900 };
    std::vector<float> bidPriceAns = { 21050, 21040, 21030, 21020, 21010 };
    std::vector<float> askVolumeAns = { 14000, 13000, 4600, 20400, 23600 };
    std::vector<float> askPriceAns = { 21060, 21070, 21080, 21090, 21100 };
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(stock.bidVolume[i], bidVolumeAns[i]);
        EXPECT_EQ(stock.bidPrice[i], bidPriceAns[i]);
        EXPECT_EQ(stock.askVolume[i], askVolumeAns[i]);
        EXPECT_EQ(stock.askPrice[i], askPriceAns[i]);
    }

    EXPECT_EQ(stock.date, "2024-01-30");
    EXPECT_EQ(stock.time, "15:00:00");
    EXPECT_EQ(stock.closeStatus, "00");
}
