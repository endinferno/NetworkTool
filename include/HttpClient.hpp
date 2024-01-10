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

    explicit HttpClient(EpollerPtr& epoller);
    void Connect(const std::string& domainName, uint16_t port = 80);
    void Request(const HttpRequest& httpReq);
    void SetMessageDecodeCallback(MessageDecodeCallback callback);
    ~HttpClient() = default;

private:
    void OnMessage(const std::string& httpMsg);

private:
    TcpClient tcpClient_;
    HttpResponse response_;
    MessageDecodeCallback callback_;
};
