#include "Connector/UdpConnector.hpp"
#include "Utils/Logger.hpp"

UdpConnector::UdpConnector(EventPollerPtr& poller)
    : Connector(poller, Socket::SocketType::UDP)
{}

bool UdpConnector::HandleConnect([[maybe_unused]] ChannelPtr& chan)
{
    INFO("Success to connect\n");
    return true;
}
