#include "SinaStockClient.hpp"
#include "Logger.hpp"

SinaStockClient::SinaStockClient(EventPollerPtr& poller)
    : httpClient_(poller)
{}

void SinaStockClient::Connect()
{
    httpClient_.Connect(sinaStockDomainName_, sinaStockDomainPort_);
}

void SinaStockClient::GetStock(const std::string& stockId)
{
    httpClient_.SetMessageDecodeCallback(
        [this](const std::string& msg) { StockDecodeCallback(msg); });
    httpClient_.Request(ConstructStockRequest(stockId));
}

HttpRequest SinaStockClient::ConstructStockRequest(const std::string& stockId)
{
    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl(fmt::format("/list={}", stockId));
    httpReq.AddHeader("Host", sinaStockDomainName_);
    httpReq.AddHeader("Referer", "http://finance.sina.com.cn");
    return httpReq;
}

void SinaStockClient::StockDecodeCallback(const std::string& msg)
{
    auto stock = stockParser_.Parse(msg);
    DEBUG("{} {}\n", stock.id, stock.name);
}
