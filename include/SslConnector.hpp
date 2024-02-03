#pragma once

#include "SslWrapper.hpp"
#include "TcpConnector.hpp"

class SslConnector : public TcpConnector
{
public:
    explicit SslConnector(EventPollerPtr& poller);
    SocketPtr CreateSocket() override;
    ~SslConnector() override = default;

private:
    void TcpConnectCallback(ChannelPtr chan);
    bool HandleSslConnect(ChannelPtr& chan);

    SslWrapper ssl_;
    NewConnectionCallback newConnectionCallback_;
};
