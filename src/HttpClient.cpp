#include "HttpClient.hpp"
#include "Logger.hpp"

HttpClient::HttpClient(EventPollerPtr& poller)
    : tcpClient_(poller)
{}

void HttpClient::Connect(const std::string& domainName, uint16_t port)
{
    tcpClient_.Connect(domainName, port);
    tcpClient_.SetOnMessageCallback(
        std::bind(&HttpClient::OnMessage, this, std::placeholders::_1));
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
