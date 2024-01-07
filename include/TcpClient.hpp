#pragma once

#include "EpollHandler.hpp"
#include "Epoller.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"

class TcpClient : public EpollHandler
{
public:
    explicit TcpClient(std::shared_ptr<Epoller>& epoller);
    void HandleErrorEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    void HandleReadEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    void HandleWriteEvent(std::shared_ptr<TcpChannel> tcpChan) override;
    ssize_t Write(const std::vector<char>& writeBuf);
    void Connect(const std::string& domainName, uint16_t port);
    ~TcpClient() = default;

private:
    void HandleNewConnection(std::shared_ptr<TcpChannel> tcpChan);

private:
    constexpr static int MAX_READ_BUFFER = 2048;

    TcpConnector tcpConnector_;
    std::shared_ptr<TcpConnection> tcpConn_;
    std::vector<char> readBuf_;
    bool isWritable;
};
