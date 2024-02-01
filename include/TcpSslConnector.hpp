#pragma once

#include "EpollHandler.hpp"
#include "SslWrapper.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"

class TcpSslConnector : public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(TcpChannelPtr)>;

    explicit TcpSslConnector(EventPollerPtr& poller);
    void HandleErrorEvent(TcpChannelPtr tcpChan) override;
    void HandleReadEvent(TcpChannelPtr tcpChan) override;
    void HandleWriteEvent(TcpChannelPtr tcpChan) override;
    void SetNewConnectionCallback(NewConnectionCallback callback);
    void Connect(const std::string& domainName, uint16_t port);
    ~TcpSslConnector() override = default;

private:
    void HandleNewSslConnection(TcpChannelPtr tcpChan);
    bool ShakeSslHands(TcpChannelPtr tcpChan);
    void ConstructSslConnectionn(TcpChannelPtr& tcpChan);

    SslWrapper ssl_;
    TcpConnector tcpConnector_;
    TcpConnection tcpConn_;
    NewConnectionCallback callback_;
};
