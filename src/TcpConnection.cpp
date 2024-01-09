#include "TcpConnection.hpp"

TcpConnection::TcpConnection()
    : tcpSock_(nullptr)
    , isConnect_(false)
{}

TcpConnection::TcpConnection(TcpSocketPtr& tcpSock)
    : tcpSock_(tcpSock)
    , isConnect_(false)
{}

void TcpConnection::Bind(const TcpSocketPtr& tcpSock)
{
    tcpSock_ = tcpSock;
}

void TcpConnection::SetConnectStatus(bool status)
{
    isConnect_ = status;
}

bool TcpConnection::GetConnectStatus() const
{
    return isConnect_;
}

void TcpConnection::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = tcpSock_->Write(writeBuf);
    if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
        throw std::runtime_error("Fail to write to socket\n");
    }
}
