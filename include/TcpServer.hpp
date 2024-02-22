#pragma once

#include "TcpAcceptor.hpp"
#include "Utils/NonCopyable.hpp"

class TcpServer : public NonCopyable, public EpollHandler
{
public:
    using OnMessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;

    explicit TcpServer(EventPollerPtr& poller);
    void Run(const IPAddress& localIp, const uint16_t& localPort);
    void SetOnMessageCallback(OnMessageCallback&& callback);

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(SocketPtr&& sock);

    constexpr static int MAX_READ_BUFFER = 2048;
    std::string readBuf_;
    TcpAcceptorPtr tcpAcceptor_;
    OnMessageCallback callback_;
};
