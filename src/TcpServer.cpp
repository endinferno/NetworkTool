#include "TcpServer.hpp"
#include "EpollHandler.hpp"
#include "Utils/Logger.hpp"

TcpServer::TcpServer(EventPollerPtr& poller)
    : EpollHandler(poller)
    , readBuf_(MAX_READ_BUFFER, 0)
    , acceptor_(std::make_shared<Acceptor>(poller))
{}

void TcpServer::Run(const IPAddress& localIp, const uint16_t& localPort)
{
    acceptor_->SetNewConnectionCallback(
        [this](SocketPtr&& sock) { HandleNewConnection(std::move(sock)); });
    acceptor_->Accept(localIp, localPort);
}

void TcpServer::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Connection closed\n");
    DelEvent(chan);
}

void TcpServer::HandleReadEvent(ChannelPtr&& chan)
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
                DEBUG("No more data\n");
                break;
            } else {
                ERROR("Fail to read {}\n", savedErr);
                break;
            }
        } else if (readBytes == 0) {
            ERROR("Connection closed\n");
            DelEvent(chan);
            break;
        }
        DEBUG("Call message callback\n");
        if (callback_ != nullptr) {
            // TODO: maybe there is a better way
            readBuf_.resize(readBytes);
            callback_(chan, readBuf_);
        }
    }
}

void TcpServer::HandleWriteEvent(ChannelPtr&& chan)
{}

void TcpServer::HandleNewConnection(SocketPtr&& sock)
{
    DEBUG("HandleNewConnection\n");
    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    AddEvent(chan,
             Pollable::READ_EVENT | Pollable::WRITE_EVENT |
                 Pollable::Event::EventEt);
}

void TcpServer::SetOnMessageCallback(OnMessageCallback&& callback)
{
    callback_ = std::move(callback);
}
