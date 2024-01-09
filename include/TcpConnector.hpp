#pragma once

#include "EpollHandler.hpp"
#include "Epoller.hpp"

class TcpConnector : public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(TcpChannelPtr)>;

    TcpConnector(EpollerPtr& epoller);
    void HandleErrorEvent(TcpChannelPtr tcpChan) override;
    void HandleReadEvent(TcpChannelPtr tcpChan) override;
    void HandleWriteEvent(TcpChannelPtr tcpChan) override;
    void SetNewConnectionCallback(NewConnectionCallback callback);
    void Connect(TcpSocketPtr& tcpSock, const std::string& domainName,
                 uint16_t port);
    ~TcpConnector() = default;

private:
    NewConnectionCallback callback_;
};
