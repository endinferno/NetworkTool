#include "UdpServer.hpp"
#include "EpollHandler.hpp"
#include "Utils/Logger.hpp"

UdpServer::UdpServer(EventPollerPtr& poller)
    : EpollHandler(poller)
    , udpAcceptor_(poller)
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void UdpServer::Run(const IPAddress& localIp, const uint16_t& localPort)
{
    udpAcceptor_.SetNewConnectionCallback(
        [this](SocketPtr&& sock) { HandleNewConnection(std::move(sock)); });
    udpAcceptor_.Accept(localIp, localPort);
}

void UdpServer::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Connection closed\n");
    DelEvent(chan);
}

void UdpServer::HandleReadEvent(ChannelPtr&& chan)
{
    auto sock = chan->GetSock();
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        ssize_t readBytes = sock->Read(readBuf_);
        DEBUG("read bytes {}\n", readBytes);
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

void UdpServer::HandleWriteEvent(ChannelPtr&& chan)
{}

void UdpServer::HandleNewConnection(SocketPtr&& sock)
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

void UdpServer::SetOnMessageCallback(OnMessageCallback&& callback)
{
    callback_ = std::move(callback);
}
