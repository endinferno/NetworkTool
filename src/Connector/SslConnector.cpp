#include "Connector/SslConnector.hpp"
#include "Utils/Logger.hpp"

SslConnector::SslConnector(EventPollerPtr& poller)
    : TcpConnector(poller)
{
    // Set callback_ with TcpConnectCallback which used to get ready to
    // construct ssl construction after tcp connection construct
    Connector::SetNewConnectionCallback(
        [this](ChannelPtr& chan) { TcpConnectCallback(chan); });
}

void SslConnector::TcpConnectCallback(ChannelPtr& chan)
{
    // Set ssl connection procedure
    SetConnectProcedure([this](ChannelPtr& chan) -> bool {
        DelEvent(chan);
        return HandleSslConnect(chan);
    });
    // Set callback_ with user-defined callback after ssl connection construct
    Connector::SetNewConnectionCallback(std::move(newConnectionCallback_));

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

void SslConnector::SetNewConnectionCallback(NewConnectionCallback&& callback)
{
    // Store user-defined callback to a new callback variable
    newConnectionCallback_ = std::move(callback);
}
