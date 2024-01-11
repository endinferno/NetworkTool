#include <codecvt>
#include <locale>
#include <stdexcept>

#include "SinaStock.hpp"

// Maybe change to libiconv in the future
std::string SinaStock::ConvertGbkToUtf8(const std::string& gbkName) const
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

void SinaStock::Parse(const std::string& msg)
{
    float tmp;
    char gbkName[20];
    std::sscanf(msg.data(),
                "var hq_str_%[^=]="
                "\"%[^,],%f,%f,%f,%f,%f,%f,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%u,%f,%u,%f,%u,%f,%u,%f,%u,%f,"
                "%[^,],%[^,],%[^\"]\"",
                id_.data(),
                gbkName,
                &todayOpenPrice_,
                &yesterdayOpenPrice_,
                &latestTxnPrice_,
                &highestTxnPrice_,
                &lowestTxnPrice_,
                &tmp,
                &tmp,
                &bidVolume_[0],
                &bidPrice_[0],
                &bidVolume_[1],
                &bidPrice_[1],
                &bidVolume_[2],
                &bidPrice_[2],
                &bidVolume_[3],
                &bidPrice_[3],
                &bidVolume_[4],
                &bidPrice_[4],
                &askVolume_[0],
                &askPrice_[0],
                &askVolume_[1],
                &askPrice_[1],
                &askVolume_[2],
                &askPrice_[2],
                &askVolume_[3],
                &askPrice_[3],
                &askVolume_[4],
                &askPrice_[4],
                date_.data(),
                time_.data(),
                closeStatus_.data());
    name_ = ConvertGbkToUtf8(gbkName);
}
