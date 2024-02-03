#include "Client.hpp"
#include "Logger.hpp"

Client::Client(EventPollerPtr& poller,
               enum Connector::ConnectorType connectorType)
    : EpollHandler(poller)
    , connector_(connectorFactory_.Create(poller, connectorType))
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void Client::HandleErrorEvent([[maybe_unused]] ChannelPtr chan)
{
    ERROR("Fail to handle client\n");
}

void Client::HandleReadEvent(ChannelPtr chan)
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

void Client::HandleWriteEvent([[maybe_unused]] ChannelPtr chan)
{
    conn_.SetConnectStatus(true);
}

void Client::Write(const std::string& writeBuf)
{
    if (!conn_.GetConnectStatus()) {
        return;
    }
    conn_.Write(writeBuf);
}

void Client::Connect(IPAddress serverIp, uint16_t serverPort)
{
    connector_->SetNewConnectionCallback(
        [this](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector_->Connect(serverIp, serverPort);
}

void Client::SetOnMessageCallback(OnMessageCallback callback)
{
    callback_ = std::move(callback);
}

void Client::HandleNewConnection(ChannelPtr chan)
{
    INFO("New connection construct\n");
    conn_.Bind(chan->GetSock());
    conn_.SetConnectStatus(true);

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
