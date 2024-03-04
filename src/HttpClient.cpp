#include "HttpClient.hpp"

HttpClient::HttpClient(EventPollerPtr& poller)
    : resolver_(std::make_shared<DnsResolver>(poller))
    , tcpClient_(poller)
{}

void HttpClient::Connect(const IPAddress& serverIp, const uint16_t& serverPort)
{
    tcpClient_.SetMessageCallback(
        [this]([[maybe_unused]] ChannelPtr& chan, const std::string& httpMsg) {
            OnMessage(httpMsg);
        });
    tcpClient_.Connect(serverIp, serverPort);
}

void HttpClient::Connect(const std::string& serverName,
                         const uint16_t& serverPort)
{
    resolver_->SetDnsMessageCallback(
        [this, serverPort](const IPAddress& serverIp) {
            Connect(serverIp, serverPort);
        });
    resolver_->RequestIp(serverName);
}

void HttpClient::Request(const HttpRequest& httpReq)
{
    tcpClient_.Write(httpReq.Stringify());
}

void HttpClient::SetMessageDecodeCallback(MessageDecodeCallback&& callback)
{
    messageDecodeCallback_ = std::move(callback);
}

void HttpClient::SetWriteCompleteCallback(
    Client::WriteCompleteCallback&& callback)
{
    tcpClient_.SetWriteCompleteCallback(std::move(callback));
}

void HttpClient::OnMessage(const std::string& httpMsg)
{
    auto httpRespOption = parser_.Parse(httpMsg);
    if (!httpRespOption.has_value()) {
        return;
    }
    auto httpResp = httpRespOption.value();
    auto statusCode = httpResp.GetStatus();
    if (statusCode != HttpResponse::HttpStatusCode::OK) {
        throw std::runtime_error(fmt::format("Wrong http status code {}\n",
                                             static_cast<int>(statusCode)));
    }

    if (messageDecodeCallback_ != nullptr) {
        messageDecodeCallback_(httpResp.GetBody());
    }
}
