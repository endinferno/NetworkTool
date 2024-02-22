#pragma once

#include "UdpAcceptor.hpp"
#include "Utils/NonCopyable.hpp"

class UdpServer : public NonCopyable, public EpollHandler
{
public:
    using OnMessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;

    explicit UdpServer(EventPollerPtr& poller);
    void Run(const IPAddress& localIp, const uint16_t& localPort);
    void SetOnMessageCallback(OnMessageCallback&& callback);
    ~UdpServer() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(SocketPtr&& sock);

    constexpr static int MAX_READ_BUFFER = 2048;
    UdpAcceptor udpAcceptor_;
    std::string readBuf_;
    OnMessageCallback callback_;
};
