#include "Connector/SslConnector.hpp"
#include "Utils/Logger.hpp"

SslConnector::SslConnector(EventPollerPtr& poller)
    : EpollHandler(poller)
    , connector_(poller)
{}

void SslConnector::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to connect ssl\n");
}

void SslConnector::HandleReadEvent([[maybe_unused]] ChannelPtr&& chan)
{
    auto sslFd = std::dynamic_pointer_cast<SslFd>(chan->GetFd());
    auto event = HandleSslConnect(sslFd);
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

void SslConnector::HandleWriteEvent(ChannelPtr&& chan)
{
    auto sslFd = std::dynamic_pointer_cast<SslFd>(chan->GetFd());
    auto event = HandleSslConnect(sslFd);
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
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

    auto sock = std::dynamic_pointer_cast<Socket>(chan->GetFd());
    auto sslFd = std::make_shared<SslFd>(sock);
    sslFd->SetFd();
    sslFd->SetConnectState();
    chan->SetFd(sslFd);
    auto event = HandleSslConnect(sslFd);
    if (event.has_value()) {
        ModEvent(chan, event.value());
        return;
    }
    if (callback_ != nullptr) {
        callback_(chan);
    }
}

std::optional<uint32_t> SslConnector::HandleSslConnect(SslFdPtr& sslFd)
{
    int ret = sslFd->ShakeHands();
    if (ret == 1) {
        INFO("Success to construct SSL connection\n");
        return std::nullopt;
    }
    int err = sslFd->GetError(ret);
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
