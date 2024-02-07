#pragma once

#include "TcpConnector.hpp"
#include "Utils/SslWrapper.hpp"

class SslConnector : public TcpConnector
{
public:
    explicit SslConnector(EventPollerPtr& poller);
    void SetNewConnectionCallback(NewConnectionCallback&& callback) override;
    ~SslConnector() override = default;

private:
    void TcpConnectCallback(ChannelPtr& chan);
    bool HandleSslConnect(ChannelPtr& chan);

    SslWrapper ssl_;
    NewConnectionCallback newConnectionCallback_;
};
