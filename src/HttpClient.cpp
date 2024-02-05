#include "HttpClient.hpp"

HttpClient::HttpClient(EventPollerPtr& poller)
    : resolver_(std::make_shared<DnsResolver>(poller))
    , tcpClient_(poller)
{}

void HttpClient::Connect(IPAddress serverIp, uint16_t serverPort)
{
    resolver_.reset();
    tcpClient_.SetOnMessageCallback([this](const std::string& httpMsg) -> bool {
        return OnMessage(httpMsg);
    });
    tcpClient_.Connect(serverIp, serverPort);
}

void HttpClient::Connect(const std::string& serverName, uint16_t serverPort)
{
    resolver_->SetDnsMessageCallback([this, serverPort](IPAddress serverIp) {
        Connect(serverIp, serverPort);
    });
    resolver_->RequestIp(serverName);
}

void HttpClient::Request(const HttpRequest& httpReq)
{
    tcpClient_.Write(httpReq.Stringify());
}

void HttpClient::SetMessageDecodeCallback(MessageDecodeCallback callback)
{
    callback_ = std::move(callback);
}

bool HttpClient::OnMessage(const std::string& httpMsg)
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
    return false;
}
