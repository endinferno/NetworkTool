#include "Connector.hpp"
#include "Channel.hpp"
#include "Logger.hpp"
#include "TcpSocket.hpp"

Connector::Connector(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

void Connector::HandleErrorEvent([[maybe_unused]] ChannelPtr chan)
{
    ERROR("Fail to connect\n");
}

void Connector::HandleReadEvent([[maybe_unused]] ChannelPtr chan)
{
    static_assert(true);
}

void Connector::HandleWriteEvent(ChannelPtr chan)
{
    auto sock = chan->GetSock();
    int opt = sock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        throw std::runtime_error(fmt::format("Fail to connect {}", opt));
    }
    INFO("Success to connect\n");
    DelEvent(chan);
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

void Connector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

void Connector::Connect(IPAddress serverIp, uint16_t serverPort)
{
    std::shared_ptr<Socket> sock = std::make_shared<TcpSocket>();
    sock->SetReuseAddr();
    sock->SetReusePort();
    sock->SetNonBlock();
    sock->Connect(serverIp, serverPort);

    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback(nullptr);
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(chan); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(chan); });

#if defined(USE_EPOLL)
    AddEvent(chan, Pollable::Event::EventOut | Pollable::Event::EventEt);
#elif defined(USE_POLL)
    AddEvent(chan, Pollable::Event::EventOut);
#elif defined(USE_SELECT)
    AddEvent(chan, Pollable::Event::EventOut);
#endif
}
