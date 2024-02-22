#include "Acceptor.hpp"
#include "Utils/Logger.hpp"

Acceptor::Acceptor(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

void Acceptor::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Fail to accept client\n");
}

void Acceptor::HandleReadEvent(ChannelPtr&& chan)
{
    auto tcpSock = std::dynamic_pointer_cast<TcpSocket>(chan->GetSock());
    DEBUG("HandleReadEvent\n");
    AcceptClient(tcpSock);
}

void Acceptor::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    static_assert(true);
}

void Acceptor::Accept(const IPAddress& localIp, const uint16_t& localPort)
{
    auto tcpSock = std::make_shared<TcpSocket>();
    tcpSock->SetReuseAddr();
    tcpSock->SetReusePort();
    tcpSock->SetNonBlock();
    tcpSock->Bind(localIp, localPort);
    tcpSock->Listen();
    INFO("Socket {} Start to listen at {}:{}\n",
         tcpSock->GetFd(),
         localIp,
         localPort);

    auto sock = std::dynamic_pointer_cast<Socket>(tcpSock);

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

void Acceptor::AcceptClient(TcpSocketPtr& tcpSock)
{
    while (true) {
        struct sockaddr_in clientAddr;

        auto clientSock = tcpSock->Accept(clientAddr);
        if (clientSock == nullptr) {
            int err = tcpSock->GetErrno();
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

        clientSock->SetReuseAddr();
        clientSock->SetReusePort();
        clientSock->SetNonBlock();

        if (callback_ != nullptr) {
            callback_(std::move(clientSock));
        }
    }
}
