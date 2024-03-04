#pragma once

#include "Client/SslClient.hpp"
#include "HttpClient.hpp"

class HttpsClient
{
public:
    enum HttpStatusCode
    {
        OK = 200,
    };
    explicit HttpsClient(EventPollerPtr& poller);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void Connect(const std::string& serverName, const uint16_t& serverPort);
    void Request(const HttpRequest& httpReq);
    void SetMessageDecodeCallback(HttpClient::MessageDecodeCallback&& callback);
    void SetWriteCompleteCallback(SslClient::WriteCompleteCallback&& callback);
    ~HttpsClient() = default;

private:
    void OnMessage(const std::string& httpMsg);

    DnsResolverPtr resolver_;
    SslClient sslClient_;
    HttpRespParser parser_;
    HttpClient::MessageDecodeCallback callback_;
};
