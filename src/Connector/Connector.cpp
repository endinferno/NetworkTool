#include "Connector/Connector.hpp"
#include "Channel.hpp"
#include "Logger.hpp"

Connector::Connector(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

void Connector::HandleErrorEvent([[maybe_unused]] ChannelPtr chan)
{
    ERROR("Fail to connect\n");
}

void Connector::HandleReadEvent([[maybe_unused]] ChannelPtr chan)
{
    if (handleConnect_ == nullptr) {
        return;
    }
    if (!handleConnect_(chan)) {
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

void Connector::HandleWriteEvent(ChannelPtr chan)
{
    if (handleConnect_ == nullptr) {
        return;
    }
    if (!handleConnect_(chan)) {
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

void Connector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

Connector::NewConnectionCallback Connector::GetNewConnectionCallback() const
{
    return callback_;
}

void Connector::SetConnectProcedure(ConnectProcedure procedure)
{
    handleConnect_ = std::move(procedure);
}

void Connector::Connect(IPAddress serverIp, uint16_t serverPort)
{
    auto sock = CreateSocket();
    sock->SetReuseAddr();
    sock->SetReusePort();
    sock->SetNonBlock();
    sock->Connect(serverIp, serverPort);

    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback([this](ChannelPtr&& chan) { HandleReadEvent(chan); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(chan); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(chan); });

#if defined(USE_EPOLL)
    AddEvent(chan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventErr | Pollable::Event::EventEt);
#elif defined(USE_POLL)
    AddEvent(chan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventErr);
#elif defined(USE_SELECT)
    AddEvent(chan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventErr);
#endif
}