#pragma once

#include "HttpClient.hpp"
#include "SinaStockParser.hpp"

class SinaStockClient
{
public:
    explicit SinaStockClient(EventPollerPtr& poller);
    void Connect();
    bool IsConnect() const;
    void GetStock(const std::string& stockId);
    ~SinaStockClient() = default;

private:
    void StockDecodeCallback(const std::string& msg);
    HttpRequest ConstructStockRequest(const std::string& stockId);

    const std::string sinaStockDomain_ = "hq.sinajs.cn";
    const std::string sinaStockIp_ = "183.60.95.165";
    const uint16_t sinaStockPort_ = 80;
    HttpClient httpClient_;
    SinaStockParser stockParser_;
};
