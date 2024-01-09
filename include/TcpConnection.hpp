#pragma once

#include "TcpSocket.hpp"

class TcpConnection
{
public:
    explicit TcpConnection(TcpSocketPtr& tcpSock);
    ssize_t Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~TcpConnection() = default;

private:
    TcpSocketPtr tcpSock_;
    bool isConnect_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
