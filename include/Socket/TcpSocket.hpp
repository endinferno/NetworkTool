#pragma once

#include <sys/socket.h>
#include <unistd.h>

#include "Socket/Socket.hpp"

class TcpSocket : public Socket
{
public:
    TcpSocket()
        : Socket(::socket(AF_INET, SOCK_STREAM, 0))
    {}
    ~TcpSocket() override
    {
        ::close(GetFd());
    }
};

using TcpSocketPtr = std::shared_ptr<TcpSocket>;
