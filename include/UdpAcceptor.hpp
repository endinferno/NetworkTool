#pragma once

#include "EpollHandler.hpp"
#include "Socket/Socket.hpp"
#include "Utils/IPAddress.hpp"
#include "Utils/NonCopyable.hpp"

class UdpAcceptor : public NonCopyable, public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(SocketPtr&&)>;

    explicit UdpAcceptor(EventPollerPtr& poller);
    void Accept(const IPAddress& localIp, const uint16_t& localPort);
    void SetNewConnectionCallback(NewConnectionCallback&& callback);

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;

    NewConnectionCallback callback_;
};

using UdpAcceptorPtr = std::shared_ptr<UdpAcceptor>();
