#pragma once

#include "Client/Client.hpp"

class SslClient : public Client
{
public:
    explicit SslClient(EventPollerPtr& poller)
        : Client(poller, Connector::ConnectorType::SSL)
    {}
    ~SslClient() override = default;
};

using SslClientPtr = std::shared_ptr<SslClient>;
