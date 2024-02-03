#include "SslConnector.hpp"
#include "Logger.hpp"
#include "TcpSocket.hpp"

SslConnector::SslConnector(EventPollerPtr& poller)
    : TcpConnector(poller)
{}

SocketPtr SslConnector::CreateSocket()
{
    newConnectionCallback_ = GetNewConnectionCallback();
    SetNewConnectionCallback(
        [this](ChannelPtr&& chan) { TcpConnectCallback(chan); });
    return std::make_shared<TcpSocket>();
}

void SslConnector::TcpConnectCallback(ChannelPtr chan)
{
    SetConnectProcedure([this](ChannelPtr&& chan) -> bool {
        DelEvent(chan);
        return HandleSslConnect(chan);
    });
    SetNewConnectionCallback(newConnectionCallback_);

    ssl_.SetFd(chan->GetSock()->GetFd());
    ssl_.SetConnectState();
    HandleSslConnect(chan);
}

bool SslConnector::HandleSslConnect(ChannelPtr& chan)
{
    int ret = ssl_.ShakeHands();
    if (ret == 1) {
        INFO("Success to construct SSL connection\n");
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
