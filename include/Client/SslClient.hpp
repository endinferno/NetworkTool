#pragma once

#include "Client/Client.hpp"
#include "Connection/Connection.hpp"
#include "Connector/SslConnector.hpp"

class SslClient : public EpollHandler
{
public:
    explicit SslClient(EventPollerPtr& poller);
    void Write(const std::string& writeBuf);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void Shutdown(ChannelPtr& chan);
    void SetMessageCallback(Client::MessageCallback&& callback);
    void SetConnectCallback(Client::ConnectCallback&& callback);
    ~SslClient() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(ChannelPtr& chan);

    constexpr static int MAX_READ_BUFFER = 2048;
    SslConnector connector_;
    ConnectionPtr conn_;
    std::string readBuf_;
    Client::MessageCallback messageCallback_;
    Client::ConnectCallback connectCallback_;
};

using SslClientPtr = std::shared_ptr<SslClient>;
