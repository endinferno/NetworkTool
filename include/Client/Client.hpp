#pragma once

#include "Connection.hpp"
#include "Connector/ConnectorFactory.hpp"

class Client : public EpollHandler
{
public:
    using OnMessageCallback = std::function<void(const std::string& msg)>;

    explicit Client(EventPollerPtr& poller,
                    enum Connector::ConnectorType connectorType);
    void HandleErrorEvent(ChannelPtr chan) override;
    void HandleReadEvent(ChannelPtr chan) override;
    void HandleWriteEvent(ChannelPtr chan) override;
    void Write(const std::string& writeBuf);
    void Connect(IPAddress serverIp, uint16_t serverPort);
    void SetOnMessageCallback(OnMessageCallback callback);
    ~Client() override = default;

private:
    void HandleNewConnection(ChannelPtr chan);

    constexpr static int MAX_READ_BUFFER = 2048;
    ConnectorFactory connectorFactory_;
    ConnectorPtr connector_;
    Connection conn_;
    std::string readBuf_;
    OnMessageCallback callback_;
};

using ClientPtr = std::shared_ptr<Client>;
