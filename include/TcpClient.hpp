#pragma once

#include "EpollHandler.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"

class TcpClient : public EpollHandler
{
public:
    using OnMessageCallback = std::function<void(const std::string& msg)>;

    explicit TcpClient(EpollerPtr& epoller);
    void HandleErrorEvent(TcpChannelPtr tcpChan) override;
    void HandleReadEvent(TcpChannelPtr tcpChan) override;
    void HandleWriteEvent(TcpChannelPtr tcpChan) override;
    void Write(const std::string& writeBuf);
    void Connect(const std::string& domainName, uint16_t port);
    void SetOnMessageCallback(OnMessageCallback callback);
    ~TcpClient() override = default;

private:
    void HandleNewConnection(TcpChannelPtr tcpChan);

private:
    constexpr static int MAX_READ_BUFFER = 2048;

    TcpConnector tcpConnector_;
    TcpConnection tcpConn_;
    std::string readBuf_;
    OnMessageCallback callback_;
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
