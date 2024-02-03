#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "Socket/Socket.hpp"

class UdpSocket : public Socket
{
public:
    UdpSocket();
    ~UdpSocket() override;
};

using UdpSocketPtr = std::shared_ptr<UdpSocket>;
