#pragma once

#include "Connection/Connection.hpp"
#include "Socket/Socket.hpp"

class TcpConnection : public Connection
{
public:
    explicit TcpConnection(SocketPtr sock);
    void Write(const std::string& writeBuf) override;
    ~TcpConnection() override = default;

private:
    SocketPtr sock_;
};
