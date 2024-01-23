#pragma once

#include "HttpClient.hpp"
#include "SinaStockParser.hpp"

class SinaStockClient
{
public:
    SinaStockClient(EventPollerPtr& poller);
    void Connect();
    bool IsConnect() const;
    void GetStock(const std::string& stockId);
    ~SinaStockClient() = default;

private:
    void StockDecodeCallback(const std::string& msg);
    HttpRequest ConstructStockRequest(const std::string& stockId);

private:
    const std::string sinaStockDomainName = "hq.sinajs.cn";
    const uint16_t sinaStockDomainPort = 80;
    HttpClient httpClient_;
    SinaStockParser stockParser_;
};
