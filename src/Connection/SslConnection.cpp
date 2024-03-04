#include "Connection/SslConnection.hpp"

SslConnection::SslConnection(SslFdPtr sslFd)
    : sslFd_(std::move(sslFd))
{}

void SslConnection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = sslFd_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}