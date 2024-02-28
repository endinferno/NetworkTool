#include "Connector/SslConnector.hpp"
#include "Utils/Logger.hpp"

SslConnector::SslConnector(EventPollerPtr& poller)
    : EpollHandler(poller)
    , connector_(poller)
    , ssl_(std::make_unique<SslWrapper>())
{}

void SslConnector::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to connect ssl\n");
}

void SslConnector::HandleReadEvent([[maybe_unused]] ChannelPtr&& chan)
{
    auto event = HandleSslConnect();
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan, std::move(ssl_));
    }
}

void SslConnector::HandleWriteEvent(ChannelPtr&& chan)
{
    auto event = HandleSslConnect();
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan, std::move(ssl_));
    }
}

void SslConnector::TcpConnectCallback(ChannelPtr& chan)
{
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    ssl_->SetFd(chan->GetFd()->GetFd());
    ssl_->SetConnectState();
    auto event = HandleSslConnect();
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan, std::move(ssl_));
    }
}

std::optional<uint32_t> SslConnector::HandleSslConnect()
{
    int ret = ssl_->ShakeHands();
    if (ret == 1) {
        INFO("Success to construct SSL connection\n");
        return std::nullopt;
    }
    int err = ssl_->GetError(ret);
    uint32_t event = Pollable::Event::EventIn | Pollable::Event::EventOut |
                     Pollable::Event::EventEt;
    if (err == SSL_ERROR_WANT_WRITE) {
        event |= Pollable::Event::EventOut;
        event &= ~Pollable::Event::EventIn;
    } else if (err == SSL_ERROR_WANT_READ) {
        event |= Pollable::Event::EventIn;
        event &= ~Pollable::Event::EventOut;
    } else {
        ERROR("Fail to shake ssl hands\n");
    }
    return event;
}

void SslConnector::Connect(const IPAddress& serverIp,
                           const uint16_t& serverPort)
{
    connector_.SetNewConnectionCallback(
        [this](ChannelPtr& chan) { TcpConnectCallback(chan); });
    connector_.Connect(serverIp, serverPort);
}

void SslConnector::SetNewConnectionCallback(SslConnectionCallback&& callback)
{
    callback_ = std::move(callback);
}

void SslConnector::Shutdown(ChannelPtr& chan)
{
    DelEvent(chan);
}
