#pragma once

#include "Client/Client.hpp"

class TcpClient : public Client
{
public:
    explicit TcpClient(EventPollerPtr& poller)
        : Client(poller, Connector::ConnectorType::TCP)
    {}
    ~TcpClient() override = default;
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
