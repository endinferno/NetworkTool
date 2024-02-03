#pragma once

#include "Client/Client.hpp"

class UdpClient : public Client
{
public:
    explicit UdpClient(EventPollerPtr& poller)
        : Client(poller, Connector::ConnectorType::UDP)
    {}
    ~UdpClient() override = default;
};

using UdpClientPtr = std::shared_ptr<UdpClient>;
