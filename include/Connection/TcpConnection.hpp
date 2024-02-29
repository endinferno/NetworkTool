#pragma once

#include "Connection/Connection.hpp"
#include "Socket/Socket.hpp"

class TcpConnection : public Connection
{
public:
    explicit TcpConnection(SocketPtr sock)
        : sock_(std::move(sock))
    {}
    void Write(const std::string& writeBuf) override
    {
        ssize_t writeBytes = sock_->Write(writeBuf);
        if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
            throw std::runtime_error("Fail to write to socket\n");
        }
    }
    ~TcpConnection() override = default;

private:
    SocketPtr sock_;
};
