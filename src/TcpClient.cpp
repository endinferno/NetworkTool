#include "TcpClient.hpp"
#include "Logger.hpp"

TcpClient::TcpClient(EventPollerPtr& poller)
    : EpollHandler(poller)
    , tcpConnector_(poller)
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void TcpClient::HandleErrorEvent([[maybe_unused]] TcpChannelPtr tcpChan)
{
    ERROR("Fail to handle client\n");
}

void TcpClient::HandleReadEvent(TcpChannelPtr tcpChan)
{
    auto tcpSock = tcpChan->GetSock();
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        ssize_t readBytes = tcpSock->Read(readBuf_);
        int savedErr = tcpSock->GetErrno();
        if (readBytes == -1) {
            if (savedErr == EINTR || savedErr == EWOULDBLOCK ||
                savedErr == EAGAIN) {
                break;
            } else {
                ERROR("Fail to read {}\n", savedErr);
                break;
            }
        } else if (readBytes == 0) {
            ERROR("Connection closed\n");
            break;
        }
        if (callback_ != nullptr) {
            // TODO: maybe there is a better way
            readBuf_.resize(readBytes);
            callback_(readBuf_);
        }
    }
}

void TcpClient::HandleWriteEvent([[maybe_unused]] TcpChannelPtr tcpChan)
{
    tcpConn_.SetConnectStatus(true);
}

void TcpClient::Write(const std::string& writeBuf)
{
    if (!tcpConn_.GetConnectStatus()) {
        return;
    }
    tcpConn_.Write(writeBuf);
}

void TcpClient::Connect(const std::string& domainName, uint16_t port)
{
    tcpConnector_.SetNewConnectionCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleNewConnection(tcpChan); });
    tcpConnector_.Connect(domainName, port);
}

void TcpClient::SetOnMessageCallback(OnMessageCallback callback)
{
    callback_ = std::move(callback);
}

void TcpClient::HandleNewConnection(TcpChannelPtr tcpChan)
{
    INFO("New connection construct\n");
    tcpConn_.Bind(tcpChan->GetSock());
    tcpConn_.SetConnectStatus(true);

    tcpChan->SetReadCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleReadEvent(tcpChan); });
    tcpChan->SetWriteCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleWriteEvent(tcpChan); });
    tcpChan->SetErrorCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleErrorEvent(tcpChan); });

#if defined(USE_EPOLL)
    AddEvent(tcpChan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventEt);
#elif defined(USE_POLL)
    AddEvent(tcpChan, Pollable::Event::EventIn);
#elif defined(USE_SELECT)
    AddEvent(tcpChan, Pollable::Event::EventIn);
#endif
}
