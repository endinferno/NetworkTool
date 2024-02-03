#pragma once

#include "Client.hpp"

class TcpClient : public Client
{
public:
    explicit TcpClient(EventPollerPtr& poller);
    ~TcpClient() override = default;
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
