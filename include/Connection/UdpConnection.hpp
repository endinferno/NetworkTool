#pragma once

#include "Connection/Connection.hpp"
#include "Socket/Socket.hpp"

class UdpConnection : public Connection
{
public:
    explicit UdpConnection(SocketPtr sock);
    void Write(const std::string& writeBuf) override;
    ~UdpConnection() override = default;

private:
    SocketPtr sock_;
};
