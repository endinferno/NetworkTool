#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "Socket/Socket.hpp"

class UdpSocket : public Socket
{
public:
    UdpSocket()
        : Socket(::socket(AF_INET, SOCK_DGRAM, 0))
    {}
    ~UdpSocket() override
    {
        ::close(GetFd());
    }
};

using UdpSocketPtr = std::shared_ptr<UdpSocket>;
