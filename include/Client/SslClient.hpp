#pragma once

#include "Client/Client.hpp"
#include "Connection/Connection.hpp"
#include "Connector/SslConnector.hpp"

class SslClient : public EpollHandler
{
public:
    using WriteCompleteCallback = std::function<void(ChannelPtr&)>;

    explicit SslClient(EventPollerPtr& poller);
    void Write(const std::string& writeBuf);
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort);
    void Shutdown(ChannelPtr& chan);
    void SetMessageCallback(Client::MessageCallback&& callback);
    void SetWriteCompleteCallback(WriteCompleteCallback&& callback);
    ~SslClient() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;
    void HandleNewConnection(ChannelPtr& chan);

    constexpr static int MAX_READ_BUFFER = 65535;
    SslConnector connector_;
    ConnectionPtr conn_;
    std::string readBuf_;
    Client::MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
};

using SslClientPtr = std::shared_ptr<SslClient>;
