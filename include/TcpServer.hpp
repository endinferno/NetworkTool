#pragma once

#include "Acceptor.hpp"
#include "Utils/NonCopyable.hpp"

class TcpServer : public NonCopyable, public EpollHandler
{
public:
    using OnMessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;

    explicit TcpServer(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void Run(const IPAddress& localIp, const uint16_t& localPort);
    void SetOnMessageCallback(OnMessageCallback&& callback);

private:
    void HandleNewConnection(SocketPtr&& sock);

    constexpr static int MAX_READ_BUFFER = 2048;
    std::string readBuf_;
    AcceptorPtr acceptor_;
    OnMessageCallback callback_;
};
