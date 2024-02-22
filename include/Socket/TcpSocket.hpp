#pragma once

#include "Socket/Socket.hpp"

class TcpSocket;

using TcpSocketPtr = std::shared_ptr<TcpSocket>;

class TcpSocket : public Socket
{
public:
    TcpSocket();
    explicit TcpSocket(int sockFd);
    void Bind(const IPAddress& localIp, const uint16_t& localPort);
    void Listen();
    SocketPtr Accept(struct sockaddr_in& clientAddr);

    ~TcpSocket() override;
};
