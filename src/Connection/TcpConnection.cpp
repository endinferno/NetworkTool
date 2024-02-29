#include "Connection/TcpConnection.hpp"

TcpConnection::TcpConnection(SocketPtr sock)
    : sock_(std::move(sock))
{}

void TcpConnection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = sock_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}
