#include "Connector/UdpConnector.hpp"
#include "Utils/Logger.hpp"

UdpConnector::UdpConnector(EventPollerPtr& poller)
    : Connector(poller, Socket::SocketType::UDP)
{
    SetConnectProcedure(
        [this](ChannelPtr&& chan) -> bool { return HandleConnect(chan); });
}

bool UdpConnector::HandleConnect([[maybe_unused]] ChannelPtr chan)
{
    DelEvent(chan);
    INFO("Success to connect\n");
    return true;
}
