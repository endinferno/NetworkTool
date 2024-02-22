#include "UdpAcceptor.hpp"
#include "Socket/UdpSocket.hpp"
#include "Utils/Logger.hpp"

UdpAcceptor::UdpAcceptor(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

void UdpAcceptor::Accept(const IPAddress& localIp, const uint16_t& localPort)
{
    auto udpSock = std::make_shared<UdpSocket>();
    udpSock->SetReuseAddr();
    udpSock->SetReusePort();
    udpSock->SetNonBlock();
    udpSock->Bind(localIp, localPort);
    udpSock->Listen();
    INFO("Socket {} Start to listen at {}:{}\n",
         udpSock->GetFd(),
         localIp,
         localPort);

    auto sock = std::dynamic_pointer_cast<Socket>(udpSock);

    auto chan = std::make_shared<Channel>(sock);
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(nullptr);
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    AddEvent(chan, Pollable::READ_EVENT | Pollable::Event::EventEt);
}

void UdpAcceptor::SetNewConnectionCallback(NewConnectionCallback&& callback)
{
    callback_ = std::move(callback);
}

void UdpAcceptor::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Fail to accept client\n");
}

void UdpAcceptor::HandleReadEvent(ChannelPtr&& chan)
{
    DEBUG("HandleReadEvent\n");
    auto udpSock = std::dynamic_pointer_cast<UdpSocket>(chan->GetSock());
    while (true) {
        struct sockaddr_in clientAddr;

        auto clientSock = udpSock->Accept(clientAddr);
        if (clientSock == nullptr) {
            int err = udpSock->GetErrno();
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

void UdpAcceptor::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    static_assert(true);
}
