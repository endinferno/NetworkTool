#pragma once

#include "EpollHandler.hpp"

class Connector : public NonCopyable, public EpollHandler
{
public:
    enum ConnectorType
    {
        UDP,
        TCP,
    };
    using NewConnectionCallback = std::function<void(ChannelPtr&)>;

    explicit Connector(EventPollerPtr& poller,
                       enum Socket::SocketType sockType);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void SetNewConnectionCallback(NewConnectionCallback&& callback);
    ~Connector() override = default;

private:
    virtual bool HandleConnect(ChannelPtr& chan) = 0;
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;

    Socket::SocketType sockType_;
    NewConnectionCallback callback_;
};

using ConnectorPtr = std::shared_ptr<Connector>;
