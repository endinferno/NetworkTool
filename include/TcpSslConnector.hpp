#pragma once

#include "Connection.hpp"
#include "Connector.hpp"
#include "EpollHandler.hpp"
#include "SslWrapper.hpp"

class TcpSslConnector : public EpollHandler
{
public:
    using NewConnectionCallback = std::function<void(ChannelPtr)>;

    explicit TcpSslConnector(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr chan) override;
    void HandleReadEvent(ChannelPtr chan) override;
    void HandleWriteEvent(ChannelPtr chan) override;
    void SetNewConnectionCallback(NewConnectionCallback callback);
    void Connect(IPAddress serverIp, uint16_t serverPort);
    ~TcpSslConnector() override = default;

private:
    void HandleNewSslConnection(ChannelPtr chan);
    bool ShakeSslHands(ChannelPtr chan);
    void ConstructSslConnectionn(ChannelPtr& chan);

    SslWrapper ssl_;
    Connector connector_;
    Connection conn_;
    NewConnectionCallback callback_;
};
