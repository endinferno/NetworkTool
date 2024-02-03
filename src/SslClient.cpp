#include "SslClient.hpp"

SslClient::SslClient(EventPollerPtr& poller)
    : Client(poller, Connector::ConnectorType::SSL)
{}
