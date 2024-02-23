#pragma once

#include <optional>

#include "TcpConnector.hpp"
#include "Utils/SslWrapper.hpp"

class SslConnector : public NonCopyable, public EpollHandler
{
public:
    using SslConnectionCallback =
        std::function<void(ChannelPtr&, std::unique_ptr<SslWrapper>&&)>;

    explicit SslConnector(EventPollerPtr& poller);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void SetNewConnectionCallback(SslConnectionCallback&& callback);
    ~SslConnector() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void TcpConnectCallback(ChannelPtr& chan);
    std::optional<uint32_t> HandleSslConnect();

    TcpConnector connector_;
    std::unique_ptr<SslWrapper> ssl_;
    SslConnectionCallback callback_;
};
