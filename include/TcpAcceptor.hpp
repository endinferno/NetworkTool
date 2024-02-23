#pragma once

#include "Acceptor.hpp"

class TcpAcceptor : public Acceptor
{
public:
    explicit TcpAcceptor(EventPollerPtr& poller)
        : Acceptor(poller, Socket::SocketType::TCP)
    {}
    ~TcpAcceptor() override = default;
};

using TcpAcceptorPtr = std::shared_ptr<TcpAcceptor>;
