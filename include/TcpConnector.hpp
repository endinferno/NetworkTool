#pragma once

#include <memory>

#include "EpollHandler.hpp"
#include "Epoller.hpp"

class TcpConnector : public EpollHandler
{
public:
    using NewConnectionCallback =
        std::function<void(std::shared_ptr<TcpChannel>)>;
    TcpConnector(std::shared_ptr<Epoller>& epoller);
    void HandleErrorEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    void HandleReadEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    void HandleWriteEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    void SetNewConnectionCallback(NewConnectionCallback callback);
    void Connect(std::shared_ptr<TcpSocket>& tcpSock);
    ~TcpConnector() = default;

private:
    NewConnectionCallback callback_;
};
