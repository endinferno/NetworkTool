#include <stdexcept>

#include "Connection.hpp"

Connection::Connection()
    : sock_(nullptr)
    , isConnect_(false)
{}

Connection::Connection(SocketPtr& sock)
    : sock_(sock)
    , isConnect_(false)
{}

void Connection::Bind(const SocketPtr& sock)
{
    sock_ = sock;
}

void Connection::SetConnectStatus(bool status)
{
    isConnect_ = status;
}

bool Connection::GetConnectStatus() const
{
    return isConnect_;
}

void Connection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = sock_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}
