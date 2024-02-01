#pragma once

#include "TcpSocket.hpp"

class TcpConnection
{
public:
    TcpConnection();
    explicit TcpConnection(TcpSocketPtr& tcpSock);
    void Bind(const TcpSocketPtr& tcpSock);
    void Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~TcpConnection() = default;

private:
    TcpSocketPtr tcpSock_;
    bool isConnect_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
