#include "TcpSslConnector.hpp"
#include "Logger.hpp"

TcpSslConnector::TcpSslConnector(EventPollerPtr& poller)
    : EpollHandler(poller)
    , connector_(poller)
{}

void TcpSslConnector::HandleErrorEvent([[maybe_unused]] ChannelPtr chan)
{
    ERROR("Fail to connect\n");
}

void TcpSslConnector::HandleReadEvent(ChannelPtr chan)
{
    DelEvent(chan);
    ConstructSslConnectionn(chan);
}

void TcpSslConnector::HandleWriteEvent(ChannelPtr chan)
{
    DelEvent(chan);
    ConstructSslConnectionn(chan);
}

void TcpSslConnector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

void TcpSslConnector::Connect(IPAddress serverIp, uint16_t serverPort)
{
    connector_.SetNewConnectionCallback(
        [this](ChannelPtr&& chan) { HandleNewSslConnection(chan); });
    connector_.Connect(serverIp, serverPort);
}

void TcpSslConnector::HandleNewSslConnection(ChannelPtr chan)
{
    INFO("New connection construct\n");
    auto sock = chan->GetSock();
    conn_.Bind(sock);
    conn_.SetConnectStatus(true);

    chan->SetReadCallback([this](ChannelPtr&& chan) { HandleReadEvent(chan); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(chan); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(chan); });

    ssl_.SetFd(sock->GetFd());
    ssl_.SetConnectState();
    ConstructSslConnectionn(chan);
}

bool TcpSslConnector::ShakeSslHands(ChannelPtr chan)
{
    int ret = ssl_.ShakeHands();
    if (ret == 1) {
        return true;
    }
    int err = ssl_.GetError(ret);
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
    AddEvent(chan, event);
    return false;
}

void TcpSslConnector::ConstructSslConnectionn(ChannelPtr& chan)
{
    bool isSslConnect = ShakeSslHands(chan);
    if (!isSslConnect) {
        return;
    }
    INFO("SSL connection construct\n");
    if (callback_ != nullptr) {
        callback_(chan);
    }
}
