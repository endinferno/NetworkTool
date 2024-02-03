#include "TcpClient.hpp"

TcpClient::TcpClient(EventPollerPtr& poller)
    : Client(poller, Connector::ConnectorType::TCP)
{}
