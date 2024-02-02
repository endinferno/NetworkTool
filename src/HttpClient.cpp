#include "HttpClient.hpp"
#include "Logger.hpp"

HttpClient::HttpClient(EventPollerPtr& poller)
    : tcpClient_(poller)
{}

void HttpClient::Connect(IPAddress serverIp, uint16_t serverPort)
{
    tcpClient_.Connect(serverIp, serverPort);
    tcpClient_.SetOnMessageCallback(
        [this](const std::string& httpMsg) { OnMessage(httpMsg); });
}

void HttpClient::Request(const HttpRequest& httpReq)
{
    tcpClient_.Write(httpReq.Stringify());
}

void HttpClient::SetMessageDecodeCallback(MessageDecodeCallback callback)
{
    callback_ = std::move(callback);
}

void HttpClient::OnMessage(const std::string& httpMsg)
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
