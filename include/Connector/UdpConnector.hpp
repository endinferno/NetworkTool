#pragma once

#include "Connector.hpp"

class UdpConnector : public Connector
{
public:
    explicit UdpConnector(EventPollerPtr& poller);
    SocketPtr CreateSocket() override;
    ~UdpConnector() override = default;

private:
    bool HandleConnect(ChannelPtr chan);
};
