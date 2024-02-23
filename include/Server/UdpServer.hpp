#pragma once

#include "Server.hpp"

class UdpServer : public Server
{
public:
    explicit UdpServer(EventPollerPtr& poller)
        : Server(poller, Acceptor::AcceptorType::UDP)
    {}
    ~UdpServer() override = default;
};
