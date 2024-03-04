#pragma once

#include "Connection/Connection.hpp"
#include "Connector/Connector.hpp"

class Client : public EpollHandler
{
public:
    using MessageCallback =
        std::function<void(ChannelPtr&, const std::string& msg)>;
    using WriteCompleteCallback = std::function<void(ChannelPtr&)>;

    explicit Client(EventPollerPtr& poller,
                    enum Connector::ConnectorType connectorType);
    void Write(const std::string& writeBuf);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void Shutdown(ChannelPtr& chan);
    void SetMessageCallback(MessageCallback&& callback);
    void SetWriteCompleteCallback(WriteCompleteCallback&& callback);
    ~Client() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(ChannelPtr& chan);

    constexpr static int MAX_READ_BUFFER = 65535;
    Connector::ConnectorType connectorType_;
    ConnectorPtr connector_;
    ConnectionPtr conn_;
    std::string readBuf_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
};

using ClientPtr = std::shared_ptr<Client>;
