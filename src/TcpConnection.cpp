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

ssize_t TcpConnection::Write(const std::string& writeBuf)
{
    return tcpSock_->Write(writeBuf);
}
