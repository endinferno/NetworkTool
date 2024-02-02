#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "TcpClient.hpp"

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
    void Request(const HttpRequest& httpReq);
    void SetMessageDecodeCallback(MessageDecodeCallback callback);
    ~HttpClient() = default;

private:
    void OnMessage(const std::string& httpMsg);

    TcpClient tcpClient_;
    HttpResponse response_;
    MessageDecodeCallback callback_;
};
