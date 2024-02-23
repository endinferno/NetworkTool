#pragma once

#include "Socket/Socket.hpp"

class TcpSocket : public Socket
{
public:
    TcpSocket();
    explicit TcpSocket(int sockFd);
    void Bind(const IPAddress& localIp, const uint16_t& localPort) override;
    void Listen() override;
    SocketPtr Accept(struct sockaddr_in& clientAddr) override;

    ~TcpSocket() override;
};

using TcpSocketPtr = std::shared_ptr<TcpSocket>;
