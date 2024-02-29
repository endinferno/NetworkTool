#pragma once

#include <stdexcept>

#include "Connection/Connection.hpp"
#include "Connection/TcpConnection.hpp"
#include "Connection/UdpConnection.hpp"
#include "Connector/Connector.hpp"

class ConnectionFactory
{
public:
    ConnectionFactory() = default;
    static ConnectionPtr Create(SocketPtr sock,
                                enum Connector::ConnectorType type)
    {
        switch (type) {
        case Connector::ConnectorType::TCP:
            return std::make_shared<TcpConnection>(sock);
        case Connector::ConnectorType::UDP:
            return std::make_shared<UdpConnection>(sock);
        default: throw std::runtime_error("No such connection type\n");
        }
    }
    ~ConnectionFactory() = default;
};
