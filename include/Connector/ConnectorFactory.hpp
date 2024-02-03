#pragma once

#include "Connector/Connector.hpp"

class ConnectorFactory
{
public:
    ConnectorFactory() = default;
    ConnectorPtr Create(EventPollerPtr& poller,
                        enum Connector::ConnectorType type);
    ~ConnectorFactory() = default;
};
