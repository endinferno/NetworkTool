#pragma once

#include "Connector.hpp"

class UdpConnector : public Connector
{
public:
    explicit UdpConnector(EventPollerPtr& poller);
    ~UdpConnector() override = default;

private:
    bool HandleConnect(ChannelPtr& chan) override;
};
