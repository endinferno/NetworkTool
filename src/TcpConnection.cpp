#include <memory>

#include "TcpConnection.hpp"

TcpConnection::TcpConnection(const std::shared_ptr<TcpSocket>& tcpSock)
    : tcpSock_(tcpSock)
    , isConnect_(false)
{}

void TcpConnection::SetConnectStatus(bool status)
{
    isConnect_ = status;
}

bool TcpConnection::GetConnectStatus() const
{
    return isConnect_;
}

ssize_t TcpConnection::Write(const std::vector<char>& writeBuf)
{
    return tcpSock_->Write(writeBuf);
}
