#pragma once

#include "Acceptor.hpp"

class UdpAcceptor : public Acceptor
{
public:
    explicit UdpAcceptor(EventPollerPtr& poller)
        : Acceptor(poller, Socket::SocketType::UDP)
    {}
    ~UdpAcceptor() override = default;
};

using UdpAcceptorPtr = std::shared_ptr<UdpAcceptor>;
