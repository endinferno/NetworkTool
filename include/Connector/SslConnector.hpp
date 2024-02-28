#pragma once

#include <optional>

#include "TcpConnector.hpp"
#include "Utils/SslFd.hpp"

class SslConnector : public NonCopyable, public EpollHandler
{
public:
    explicit SslConnector(EventPollerPtr& poller);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void Shutdown(ChannelPtr& chan);
    void SetNewConnectionCallback(Connector::NewConnectionCallback&& callback);
    ~SslConnector() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void TcpConnectCallback(ChannelPtr& chan);
    std::optional<uint32_t> HandleSslConnect(SslFdPtr& sslFd);

    TcpConnector connector_;
    Connector::NewConnectionCallback callback_;
};
