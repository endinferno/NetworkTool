#include "Connection/UdpConnection.hpp"

UdpConnection::UdpConnection(SocketPtr sock)
    : sock_(std::move(sock))
{}

void UdpConnection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = sock_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}
