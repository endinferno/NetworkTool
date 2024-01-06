#include "TcpConnection.hpp"


TcpConnection::TcpConnection(const std::shared_ptr<TcpSocket>& sock)
    : tcpSock_(sock)
    , readCallback_(nullptr)
    , writeCallback_(nullptr)
    , errorCallback_(nullptr)
{}

void TcpConnection::SetReadCallback(EventCallback callback)
{
    readCallback_ = std::move(callback);
}

void TcpConnection::SetWriteCallback(EventCallback callback)
{
    writeCallback_ = std::move(callback);
}

void TcpConnection::SetErrorCallback(EventCallback callback)
{
    errorCallback_ = std::move(callback);
}

void TcpConnection::OnReadable()
{
    if (readCallback_ != nullptr) {
        readCallback_(shared_from_this());
    }
}

void TcpConnection::OnWritable()
{
    if (writeCallback_ != nullptr) {
        writeCallback_(shared_from_this());
    }
}

void TcpConnection::OnErrorable()
{
    if (errorCallback_ != nullptr) {
        errorCallback_(shared_from_this());
    }
}

void TcpConnection::SetEvent(uint32_t event)
{
    epollEvt_ = event;
}

uint32_t TcpConnection::GetEvent() const
{
    return epollEvt_;
}

std::shared_ptr<TcpSocket> TcpConnection::GetSock() const
{
    return tcpSock_;
}
