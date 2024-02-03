#include <memory>
#include <stdexcept>

#include "Connector/ConnectorFactory.hpp"
#include "Connector/SslConnector.hpp"
#include "Connector/TcpConnector.hpp"
#include "Connector/UdpConnector.hpp"

ConnectorPtr ConnectorFactory::Create(EventPollerPtr& poller,
                                      enum Connector::ConnectorType type)
{
    switch (type) {
    case Connector::ConnectorType::TCP:
        return std::make_unique<TcpConnector>(poller);
    case Connector::ConnectorType::UDP:
        return std::make_unique<UdpConnector>(poller);
    case Connector::ConnectorType::SSL:
        return std::make_unique<SslConnector>(poller);
    default: throw std::runtime_error("No such socket type\n");
    }
}
