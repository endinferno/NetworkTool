#include "TcpChannel.hpp"

TcpChannel::TcpChannel(const std::shared_ptr<TcpSocket>& sock)
    : tcpSock_(sock)
    , readCallback_(nullptr)
    , writeCallback_(nullptr)
    , errorCallback_(nullptr)
{}

void TcpChannel::SetReadCallback(EventCallback callback)
{
    readCallback_ = std::move(callback);
}

void TcpChannel::SetWriteCallback(EventCallback callback)
{
    writeCallback_ = std::move(callback);
}

void TcpChannel::SetErrorCallback(EventCallback callback)
{
    errorCallback_ = std::move(callback);
}

void TcpChannel::OnReadable()
{
    if (readCallback_ != nullptr) {
        readCallback_(shared_from_this());
    }
}

void TcpChannel::OnWritable()
{
    if (writeCallback_ != nullptr) {
        writeCallback_(shared_from_this());
    }
}

void TcpChannel::OnErrorable()
{
    if (errorCallback_ != nullptr) {
        errorCallback_(shared_from_this());
    }
}

void TcpChannel::SetEvent(uint32_t event)
{
    epollEvt_ = event;
}

uint32_t TcpChannel::GetEvent() const
{
    return epollEvt_;
}

std::shared_ptr<TcpSocket> TcpChannel::GetSock() const
{
    return tcpSock_;
}
