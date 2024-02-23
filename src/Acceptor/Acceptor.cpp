#include "Acceptor/Acceptor.hpp"
#include "Socket/SocketFactory.hpp"
#include "Utils/Logger.hpp"

Acceptor::Acceptor(EventPollerPtr& poller, Socket::SocketType sockType)
    : EpollHandler(poller)
    , sockType_(sockType)
{}

void Acceptor::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Fail to accept client\n");
}

void Acceptor::HandleReadEvent(ChannelPtr&& chan)
{
    auto sock = chan->GetSock();
    while (true) {
        struct sockaddr_in clientAddr;

        auto clientSock = sock->Accept(clientAddr);
        if (clientSock == nullptr) {
            int err = sock->GetErrno();
            if ((err == EAGAIN) || (err == EWOULDBLOCK)) {
                INFO("Success to accept all coming connections\n");
                break;
            } else {
                ERROR("Fail to accept\n");
                continue;
            }
        }

        INFO("Accpet connection {} from {}:{}\n",
             clientSock->GetFd(),
             inet_ntoa(clientAddr.sin_addr),
             ntohs(clientAddr.sin_port));

        if (callback_ != nullptr) {
            callback_(std::move(clientSock));
        }
    }
}

void Acceptor::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    static_assert(true);
}

void Acceptor::Accept(const IPAddress& localIp, const uint16_t& localPort)
{
    auto sock = SocketFactory::Create(sockType_);
    sock->SetReuseAddr();
    sock->SetReusePort();
    sock->SetNonBlock();
    sock->Bind(localIp, localPort);
    sock->Listen();
    INFO("Socket {} Start to listen at {}:{}\n",
         sock->GetFd(),
         localIp,
         localPort);

    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(nullptr);
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    AddEvent(chan, Pollable::READ_EVENT | Pollable::Event::EventEt);
}

void Acceptor::SetNewConnectionCallback(NewConnectionCallback&& callback)
{
    callback_ = std::move(callback);
}
