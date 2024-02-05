#pragma once

#include "Client/TcpClient.hpp"
#include "Dns/DnsResolver.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpClient
{
public:
    enum HttpStatusCode
    {
        OK = 200,
    };
    using MessageDecodeCallback = std::function<void(const std::string& msg)>;

    explicit HttpClient(EventPollerPtr& poller);
    void Connect(IPAddress serverIp, uint16_t serverPort);
    void Connect(const std::string& serverName, uint16_t serverPort);
    void Request(const HttpRequest& httpReq);
    void SetMessageDecodeCallback(MessageDecodeCallback callback);
    ~HttpClient() = default;

private:
    bool OnMessage(const std::string& httpMsg);

    DnsResolverPtr resolver_;
    TcpClient tcpClient_;
    HttpResponse response_;
    MessageDecodeCallback callback_;
};
