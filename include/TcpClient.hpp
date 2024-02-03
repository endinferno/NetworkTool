#pragma once

#include "Connection.hpp"
#include "Connector/Connector.hpp"

class TcpClient : public EpollHandler
{
public:
    using OnMessageCallback = std::function<void(const std::string& msg)>;

    explicit TcpClient(EventPollerPtr& poller);
    void HandleErrorEvent(ChannelPtr chan) override;
    void HandleReadEvent(ChannelPtr chan) override;
    void HandleWriteEvent(ChannelPtr chan) override;
    void Write(const std::string& writeBuf);
    void Connect(IPAddress serverIp, uint16_t serverPort);
    void SetOnMessageCallback(OnMessageCallback callback);
    ~TcpClient() override = default;

private:
    void HandleNewConnection(ChannelPtr chan);

    constexpr static int MAX_READ_BUFFER = 2048;
    std::unique_ptr<Connector> connector_;
    Connection conn_;
    std::string readBuf_;
    OnMessageCallback callback_;
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
