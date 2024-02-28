#include <memory>
#include <sys/socket.h>

#include "Connector/TcpConnector.hpp"
#include "Utils/Logger.hpp"

TcpConnector::TcpConnector(EventPollerPtr& poller)
    : Connector(poller, Socket::SocketType::TCP)
{}

bool TcpConnector::HandleConnect(ChannelPtr& chan)
{
    auto sock = std::dynamic_pointer_cast<Socket>(chan->GetFd());
    int opt = sock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        ERROR("Fail to connect {}", opt);
        return false;
    }
    DelEvent(chan);
    INFO("Success to connect\n");
    return true;
}
