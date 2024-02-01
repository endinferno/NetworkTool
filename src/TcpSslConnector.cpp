#include "TcpSslConnector.hpp"
#include "Logger.hpp"

TcpSslConnector::TcpSslConnector(EventPollerPtr& poller)
    : EpollHandler(poller)
    , tcpConnector_(poller)
{}

void TcpSslConnector::HandleErrorEvent([[maybe_unused]] TcpChannelPtr tcpChan)
{
    ERROR("Fail to connect\n");
}

void TcpSslConnector::HandleReadEvent(TcpChannelPtr tcpChan)
{
    DelEvent(tcpChan);
    ConstructSslConnectionn(tcpChan);
}

void TcpSslConnector::HandleWriteEvent(TcpChannelPtr tcpChan)
{
    DelEvent(tcpChan);
    ConstructSslConnectionn(tcpChan);
}

void TcpSslConnector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

void TcpSslConnector::Connect(const std::string& domainName, uint16_t port)
{
    tcpConnector_.SetNewConnectionCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleNewSslConnection(tcpChan); });
    tcpConnector_.Connect(domainName, port);
}

void TcpSslConnector::HandleNewSslConnection(TcpChannelPtr tcpChan)
{
    INFO("New connection construct\n");
    auto tcpSock = tcpChan->GetSock();
    tcpConn_.Bind(tcpSock);
    tcpConn_.SetConnectStatus(true);

    tcpChan->SetReadCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleReadEvent(tcpChan); });
    tcpChan->SetWriteCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleWriteEvent(tcpChan); });
    tcpChan->SetErrorCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleErrorEvent(tcpChan); });

    ssl_.SetFd(tcpSock->GetFd());
    ssl_.SetConnectState();
    ConstructSslConnectionn(tcpChan);
}

bool TcpSslConnector::ShakeSslHands(TcpChannelPtr tcpChan)
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
    AddEvent(tcpChan, event);
    return false;
}

void TcpSslConnector::ConstructSslConnectionn(TcpChannelPtr& tcpChan)
{
    bool isSslConnect = ShakeSslHands(tcpChan);
    if (!isSslConnect) {
        return;
    }
    INFO("SSL connection construct\n");
    if (callback_ != nullptr) {
        callback_(tcpChan);
    }
}
