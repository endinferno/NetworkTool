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

    const std::string sinaStockDomainName_ = "hq.sinajs.cn";
    const uint16_t sinaStockDomainPort_ = 80;
    HttpClient httpClient_;
    SinaStockParser stockParser_;
};
