#pragma once

#include <memory>

#include "EpollHandler.hpp"
#include "Epoller.hpp"

class TcpConnector : public EpollHandler
{
public:
    TcpConnector(std::shared_ptr<Epoller>& epoller);
    void HandleErrorEvent(std::shared_ptr<TcpConnection>& tcpConn) override;
    void HandleReadEvent(std::shared_ptr<TcpConnection>& tcpConn) override;
    void HandleWriteEvent(std::shared_ptr<TcpConnection>& tcpConn) override;
    std::shared_ptr<TcpConnection> Connect(std::shared_ptr<TcpSocket>& tcpSock);
    ~TcpConnector() = default;
};
