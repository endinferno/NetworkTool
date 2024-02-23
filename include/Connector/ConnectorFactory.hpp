#pragma once

#include <memory>
#include <stdexcept>

#include "Connector/TcpConnector.hpp"
#include "Connector/UdpConnector.hpp"

class ConnectorFactory
{
public:
    ConnectorFactory() = default;
    static ConnectorPtr Create(EventPollerPtr& poller,
                               enum Connector::ConnectorType type)
    {
        switch (type) {
        case Connector::ConnectorType::TCP:
            return std::make_unique<TcpConnector>(poller);
        case Connector::ConnectorType::UDP:
            return std::make_unique<UdpConnector>(poller);
        default: throw std::runtime_error("No such socket type\n");
        }
    }
    ~ConnectorFactory() = default;
};
