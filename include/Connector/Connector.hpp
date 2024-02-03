#pragma once

#include "EpollHandler.hpp"

class Connector : public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(ChannelPtr)>;
    using ConnectProcedure = std::function<bool(ChannelPtr)>;

    explicit Connector(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr chan) override;
    void HandleReadEvent(ChannelPtr chan) override;
    void HandleWriteEvent(ChannelPtr chan) override;
    virtual void SetNewConnectionCallback(NewConnectionCallback callback);
    [[nodiscard]] NewConnectionCallback GetNewConnectionCallback() const;
    void Connect(IPAddress serverIp, uint16_t serverPort);
    ~Connector() override = default;
    virtual SocketPtr CreateSocket() = 0;

protected:
    void SetConnectProcedure(ConnectProcedure procedure);

private:
    NewConnectionCallback callback_;
    ConnectProcedure handleConnect_;
};

using ConnectorPtr = std::unique_ptr<Connector>;
