#pragma once

#include <stdexcept>

#include "Socket/TcpSocket.hpp"
#include "Socket/UdpSocket.hpp"

class SocketFactory
{
public:
    SocketFactory() = default;
    static SocketPtr Create(enum Socket::SocketType type)
    {
        switch (type) {
        case Socket::SocketType::TCP: return std::make_shared<TcpSocket>();
        case Socket::SocketType::UDP: return std::make_shared<UdpSocket>();
        default: throw std::runtime_error("No such socket type\n");
        }
    }
    ~SocketFactory() = default;
};
