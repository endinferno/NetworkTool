#pragma once

#include "EpollHandler.hpp"
#include "Socket/TcpSocket.hpp"

class TcpAcceptor : public NonCopyable, public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(SocketPtr&&)>;

    explicit TcpAcceptor(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void Accept(const IPAddress& localIp, const uint16_t& localPort);
    void SetNewConnectionCallback(NewConnectionCallback&& callback);

private:
    void AcceptClient(TcpSocketPtr& tcpSock);
    NewConnectionCallback callback_;
};

using TcpAcceptorPtr = std::shared_ptr<TcpAcceptor>;
