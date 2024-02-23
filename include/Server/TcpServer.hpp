#pragma once

#include "Server.hpp"

class TcpServer : public Server
{
public:
    explicit TcpServer(EventPollerPtr& poller)
        : Server(poller, Acceptor::AcceptorType::TCP)
    {}
    ~TcpServer() override = default;
};
