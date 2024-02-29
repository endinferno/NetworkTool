#include "HttpsClient.hpp"

HttpsClient::HttpsClient(EventPollerPtr& poller)
    : resolver_(std::make_shared<DnsResolver>(poller))
    , sslClient_(poller)
{}

void HttpsClient::Connect(const IPAddress& serverIp, const uint16_t& serverPort)
{
    resolver_.reset();
    sslClient_.SetMessageCallback(
        [this]([[maybe_unused]] ChannelPtr& chan, const std::string& httpMsg) {
            OnMessage(httpMsg);
        });
    sslClient_.Connect(serverIp, serverPort);
}

void HttpsClient::Connect(const std::string& serverName,
                          const uint16_t& serverPort)
{
    resolver_->SetDnsMessageCallback(
        [this, serverPort](const IPAddress& serverIp) {
            Connect(serverIp, serverPort);
        });
    resolver_->RequestIp(serverName);
}

void HttpsClient::Request(const HttpRequest& httpReq)
{
    sslClient_.Write(httpReq.Stringify());
}

void HttpsClient::SetMessageDecodeCallback(
    HttpClient::MessageDecodeCallback&& callback)
{
    callback_ = std::move(callback);
}

void HttpsClient::OnMessage(const std::string& httpMsg)
{
    response_.Parse(httpMsg);
    int statusCode = response_.GetStatus();
    if (statusCode != HttpStatusCode::OK) {
        throw std::runtime_error(
            fmt::format("Wrong http status code {}\n", statusCode));
    }

    if (callback_ != nullptr) {
        callback_(response_.GetBody());
    }
}
