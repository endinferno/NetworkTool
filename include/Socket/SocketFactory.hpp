#pragma once

#include "Socket/Socket.hpp"

class SocketFactory
{
public:
    SocketFactory() = default;
    SocketPtr Create(enum Socket::SocketType type);
    ~SocketFactory() = default;
};
