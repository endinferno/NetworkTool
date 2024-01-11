#include <codecvt>
#include <locale>
#include <stdexcept>

#include "SinaStockParser.hpp"

// Maybe change to libiconv in the future
std::string SinaStockParser::ConvertGbkToUtf8(const std::string& gbkName) const
{
    std::vector<wchar_t> buff(gbkName.size());
    std::locale loc("zh_CN.GB18030");
    wchar_t* pwszNext = nullptr;
    const char* pszNext = nullptr;
    mbstate_t state = {};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t>>(loc).in(
        state,
        gbkName.data(),
        gbkName.data() + gbkName.size(),
        pszNext,
        buff.data(),
        buff.data() + buff.size(),
        pwszNext);

    if (std::codecvt_base::ok == res) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
    }

    throw std::runtime_error("Fail to convert from GBK to UTF-8\n");
}


Stock SinaStockParser::Parse(const std::string& msg)
{
    float tmp;
    Stock stock;
    std::sscanf(msg.data(),
                "var hq_str_%[^=]="
                "\"%[^,],%f,%f,%f,%f,%f,%f,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%[^,],%[^,],%[^\"]\"",
                stock.id.data(),
                gbkName_.data(),
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
    stock.name = ConvertGbkToUtf8(gbkName_.data());
    return stock;
}
