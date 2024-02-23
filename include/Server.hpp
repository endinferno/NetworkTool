#pragma once

#include "Acceptor/Acceptor.hpp"

class Server : public NonCopyable, public EpollHandler
{
public:
    using OnMessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;

    explicit Server(EventPollerPtr& poller,
                    enum Acceptor::AcceptorType acceptorType);
    void Run(const IPAddress& localIp, const uint16_t& localPort);
    void SetOnMessageCallback(OnMessageCallback&& callback);
    ~Server() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(SocketPtr&& sock);

    constexpr static int MAX_READ_BUFFER = 2048;
    AcceptorPtr acceptor_;
    std::string readBuf_;
    OnMessageCallback callback_;
};
