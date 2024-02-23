#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "Socket/Socket.hpp"

class UdpSocket : public Socket
{
public:
    UdpSocket();
    void Bind(const IPAddress& localIp, const uint16_t& localPort) override;
    void Listen() override;
    SocketPtr Accept(struct sockaddr_in& clientAddr) override;

    ~UdpSocket() override;

private:
    std::pair<IPAddress, uint16_t> serverAddr_;
};

using UdpSocketPtr = std::shared_ptr<UdpSocket>;
