#pragma once

#include "Socket/Socket.hpp"

class TcpSocket : public Socket
{
public:
    TcpSocket();
    ~TcpSocket() override;
};

using TcpSocketPtr = std::shared_ptr<TcpSocket>;
