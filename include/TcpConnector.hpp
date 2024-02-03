#pragma once

#include "Connector.hpp"

class TcpConnector : public Connector
{
public:
    explicit TcpConnector(EventPollerPtr& poller);
    SocketPtr CreateSocket() override;
    ~TcpConnector() override = default;

private:
    bool HandleConnect(ChannelPtr chan);
};
