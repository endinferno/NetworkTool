#pragma once

#include "EpollHandler.hpp"

class TcpAcceptor : public NonCopyable, public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(SocketPtr&&)>;

    explicit TcpAcceptor(EventPollerPtr& poller);
    void Accept(const IPAddress& localIp, const uint16_t& localPort);
    void SetNewConnectionCallback(NewConnectionCallback&& callback);
    ~TcpAcceptor() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;

    NewConnectionCallback callback_;
};

using TcpAcceptorPtr = std::shared_ptr<TcpAcceptor>;
