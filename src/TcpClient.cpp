#include "TcpClient.hpp"
#include "Logger.hpp"

TcpClient::TcpClient(EventPollerPtr& poller)
    : EpollHandler(poller)
    , connector_(poller)
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void TcpClient::HandleErrorEvent([[maybe_unused]] ChannelPtr chan)
{
    ERROR("Fail to handle client\n");
}

void TcpClient::HandleReadEvent(ChannelPtr chan)
{
    auto sock = chan->GetSock();
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        ssize_t readBytes = sock->Read(readBuf_);
        int savedErr = sock->GetErrno();
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

void TcpClient::HandleWriteEvent([[maybe_unused]] ChannelPtr chan)
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
    connector_.SetNewConnectionCallback(
        [this](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector_.Connect(domainName, port);
}

void TcpClient::SetOnMessageCallback(OnMessageCallback callback)
{
    callback_ = std::move(callback);
}

void TcpClient::HandleNewConnection(ChannelPtr chan)
{
    INFO("New connection construct\n");
    tcpConn_.Bind(chan->GetSock());
    tcpConn_.SetConnectStatus(true);

    chan->SetReadCallback([this](ChannelPtr&& chan) { HandleReadEvent(chan); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(chan); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(chan); });

#if defined(USE_EPOLL)
    AddEvent(chan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventEt);
#elif defined(USE_POLL)
    AddEvent(chan, Pollable::Event::EventIn);
#elif defined(USE_SELECT)
    AddEvent(chan, Pollable::Event::EventIn);
#endif
}
