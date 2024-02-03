#pragma once

#include "Client.hpp"

class SslClient : public Client
{
public:
    explicit SslClient(EventPollerPtr& poller);
    ~SslClient() override = default;
};

using SslClientPtr = std::shared_ptr<SslClient>;
