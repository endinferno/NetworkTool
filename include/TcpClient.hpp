#pragma once

#include "EpollHandler.hpp"
#include "Epoller.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"

class TcpClient : public EpollHandler
{
public:
    explicit TcpClient(EpollerPtr& epoller);
    void HandleErrorEvent(TcpChannelPtr tcpChan) override;
    void HandleReadEvent(TcpChannelPtr tcpChan) override;
    void HandleWriteEvent(TcpChannelPtr tcpChan) override;
    ssize_t Write(const std::string& writeBuf);
    void Connect(const std::string& domainName, uint16_t port);
    ~TcpClient() = default;

private:
    void HandleNewConnection(TcpChannelPtr tcpChan);

private:
    constexpr static int MAX_READ_BUFFER = 2048;

    TcpConnector tcpConnector_;
    TcpConnectionPtr tcpConn_;
    std::string readBuf_;
    bool isWritable;
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
