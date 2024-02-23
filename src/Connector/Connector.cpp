#include "Connector/Connector.hpp"
#include "Channel.hpp"
#include "Socket/SocketFactory.hpp"
#include "Utils/Logger.hpp"

Connector::Connector(EventPollerPtr& poller, enum Socket::SocketType sockType)
    : EpollHandler(poller)
    , sockType_(sockType)
{}

void Connector::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to connect\n");
}

void Connector::HandleReadEvent([[maybe_unused]] ChannelPtr&& chan)
{
    // Do nothing
}

void Connector::HandleWriteEvent(ChannelPtr&& chan)
{
    if (!HandleConnect(chan)) {
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

void Connector::SetNewConnectionCallback(NewConnectionCallback&& callback)
{
    callback_ = std::move(callback);
}

void Connector::Connect(const IPAddress& serverIp, const uint16_t& serverPort)
{
    auto sock = SocketFactory::Create(sockType_);
    sock->SetReuseAddr();
    sock->SetReusePort();
    sock->SetNonBlock();
    sock->Connect(serverIp, serverPort);

    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback(nullptr);
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    AddEvent(chan,
             Pollable::WRITE_EVENT | Pollable::ERROR_EVENT |
                 Pollable::Event::EventEt);
}
