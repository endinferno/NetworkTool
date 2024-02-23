#pragma once

#include <stdexcept>

#include "Acceptor/TcpAcceptor.hpp"
#include "Acceptor/UdpAcceptor.hpp"

class AcceptorFactory
{
public:
    AcceptorFactory() = default;
    static AcceptorPtr Create(EventPollerPtr& poller,
                              enum Acceptor::AcceptorType type)
    {
        switch (type) {
        case Acceptor::AcceptorType::TCP:
            return std::make_shared<TcpAcceptor>(poller);
        case Acceptor::AcceptorType::UDP:
            return std::make_shared<UdpAcceptor>(poller);
        default: throw std::runtime_error("No such acceptor type\n");
        }
    }
    ~AcceptorFactory() = default;
};
