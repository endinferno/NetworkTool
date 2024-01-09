#pragma once

#include <memory>

#include "TcpSocket.hpp"

class TcpConnection
{
public:
    explicit TcpConnection(const std::shared_ptr<TcpSocket>& tcpSock);
    ssize_t Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~TcpConnection() = default;

private:
    std::shared_ptr<TcpSocket> tcpSock_;
    bool isConnect_;
};
