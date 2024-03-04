#include "Connection.hpp"

Connection::Connection(PosixFdPtr posixFd)
    : posixFd_(std::move(posixFd))
{}

void Connection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = posixFd_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}

void Connection::SetConnectStatus(bool status)
{
    isConnect_ = status;
}

bool Connection::GetConnectStatus() const
{
    return isConnect_;
}
