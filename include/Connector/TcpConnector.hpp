#pragma once

#include "Connector.hpp"

class TcpConnector : public Connector
{
public:
    explicit TcpConnector(EventPollerPtr& poller);
    ~TcpConnector() override = default;

private:
    bool HandleConnect(ChannelPtr chan);
};
