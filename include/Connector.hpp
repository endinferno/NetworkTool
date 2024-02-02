#pragma once

#include "EpollHandler.hpp"

class Connector : public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(ChannelPtr)>;

    explicit Connector(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr chan) override;
    void HandleReadEvent(ChannelPtr chan) override;
    void HandleWriteEvent(ChannelPtr chan) override;
    void SetNewConnectionCallback(NewConnectionCallback callback);
    void Connect(IPAddress serverIp, uint16_t serverPort);
    ~Connector() override = default;

private:
    NewConnectionCallback callback_;
};
