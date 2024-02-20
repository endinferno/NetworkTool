#pragma once

#include "EpollHandler.hpp"

class Connector : public EpollHandler
{
public:
    enum ConnectorType
    {
        UDP,
        TCP,
        SSL
    };
    using NewConnectionCallback = std::function<void(ChannelPtr&)>;
    using ConnectProcedure = std::function<bool(ChannelPtr&)>;

    explicit Connector(EventPollerPtr& poller,
                       enum Socket::SocketType sockType);
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    virtual void SetNewConnectionCallback(NewConnectionCallback&& callback);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    ~Connector() override = default;

protected:
    void SetConnectProcedure(ConnectProcedure&& procedure);

private:
    Socket::SocketType sockType_;
    NewConnectionCallback callback_;
    ConnectProcedure handleConnect_;
};

using ConnectorPtr = std::unique_ptr<Connector>;
