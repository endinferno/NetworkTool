#pragma once

#include <memory>

#include "TcpClient.hpp"

class TcpConnector
{
public:
    TcpConnector(std::shared_ptr<TcpClient>& tcpClient);
    void HandleErrorEvent(std::shared_ptr<TcpConnection>& tcpConn);
    void HandleReadEvent(std::shared_ptr<TcpConnection>& tcpConn);
    void HandleWriteEvent(std::shared_ptr<TcpConnection>& tcpConn);
    std::shared_ptr<TcpConnection> Connect(std::shared_ptr<TcpSocket>& tcpSock);
    ~TcpConnector() = default;

private:
    std::shared_ptr<TcpClient> tcpClient_;
};
