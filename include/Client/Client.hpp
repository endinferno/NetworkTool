#pragma once

#include "Connection.hpp"
#include "Connector/ConnectorFactory.hpp"

class Client : public EpollHandler
{
public:
    using OnMessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;
    using ConnectDoneCallback = std::function<void(ChannelPtr&)>;

    explicit Client(EventPollerPtr& poller,
                    enum Connector::ConnectorType connectorType);
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void Write(const std::string& writeBuf);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void SetOnMessageCallback(OnMessageCallback&& callback);
    void SetConnectDoneCallback(ConnectDoneCallback&& callback);
    ~Client() override = default;

private:
    void HandleNewConnection(ChannelPtr& chan);

    constexpr static int MAX_READ_BUFFER = 2048;
    Connector::ConnectorType connectorType_;
    ConnectorFactory connectorFactory_;
    ConnectorPtr connector_;
    ConnectionPtr conn_;
    std::string readBuf_;
    OnMessageCallback onMessageCallback_;
    ConnectDoneCallback connectDoneCallback_;
};

using ClientPtr = std::shared_ptr<Client>;
