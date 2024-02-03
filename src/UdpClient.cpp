#include "UdpClient.hpp"

UdpClient::UdpClient(EventPollerPtr& poller)
    : Client(poller, Connector::ConnectorType::UDP)
{}
