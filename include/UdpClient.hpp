#pragma once

#include "Client.hpp"

class UdpClient : public Client
{
public:
    explicit UdpClient(EventPollerPtr& poller);
    ~UdpClient() override = default;
};

using UdpClientPtr = std::shared_ptr<UdpClient>;
