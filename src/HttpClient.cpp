#include "HttpClient.hpp"
#include "Logger.hpp"

HttpClient::HttpClient(EpollerPtr& epoller)
    : tcpClient_(epoller)
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

void HttpClient::OnMessage(const std::string& httpMsg)
{
    DEBUG("{}\n", httpMsg);
}
