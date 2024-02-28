#pragma once

#include "EpollHandler.hpp"
#include "Socket/Socket.hpp"

class Acceptor : public NonCopyable, public EpollHandler
{
public:
    enum AcceptorType
    {
        TCP,
        UDP
    };
    using NewConnectionCallback = std::function<void(SocketPtr&&)>;

    explicit Acceptor(EventPollerPtr& poller, Socket::SocketType sockType);
    void Accept(const IPAddress& localIp, const uint16_t& localPort);
    void SetNewConnectionCallback(NewConnectionCallback&& callback);
    ~Acceptor() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;

    Socket::SocketType sockType_;
    NewConnectionCallback callback_;
};

using AcceptorPtr = std::shared_ptr<Acceptor>;
