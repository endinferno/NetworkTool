#include <sys/socket.h>

#include "Connector/TcpConnector.hpp"
#include "Logger.hpp"
#include "Socket/TcpSocket.hpp"

TcpConnector::TcpConnector(EventPollerPtr& poller)
    : Connector(poller)
{
    SetConnectProcedure(
        [this](ChannelPtr&& chan) -> bool { return HandleConnect(chan); });
}

SocketPtr TcpConnector::CreateSocket()
{
    return std::make_shared<TcpSocket>();
}

bool TcpConnector::HandleConnect(ChannelPtr chan)
{
    auto sock = chan->GetSock();
    int opt = sock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        ERROR("Fail to connect {}", opt);
        return false;
    }
    DelEvent(chan);
    INFO("Success to connect\n");
    return true;
}
