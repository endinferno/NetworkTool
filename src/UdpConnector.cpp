#include "UdpConnector.hpp"
#include "Logger.hpp"
#include "Socket/UdpSocket.hpp"

UdpConnector::UdpConnector(EventPollerPtr& poller)
    : Connector(poller)
{
    SetConnectProcedure(
        [this](ChannelPtr&& chan) -> bool { return HandleConnect(chan); });
}

SocketPtr UdpConnector::CreateSocket()
{
    return std::make_shared<UdpSocket>();
}

bool UdpConnector::HandleConnect([[maybe_unused]] ChannelPtr chan)
{
    INFO("Success to connect\n");
    return true;
}
