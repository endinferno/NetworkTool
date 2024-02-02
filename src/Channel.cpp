#include "Channel.hpp"

Channel::Channel(SocketPtr& sock)
    : sock_(sock)
    , readCallback_(nullptr)
    , writeCallback_(nullptr)
    , errorCallback_(nullptr)
{}

void Channel::SetReadCallback(EventCallback callback)
{
    readCallback_ = std::move(callback);
}

void Channel::SetWriteCallback(EventCallback callback)
{
    writeCallback_ = std::move(callback);
}

void Channel::SetErrorCallback(EventCallback callback)
{
    errorCallback_ = std::move(callback);
}

void Channel::OnReadable()
{
    if (readCallback_ != nullptr) {
        readCallback_(shared_from_this());
    }
}

void Channel::OnWritable()
{
    if (writeCallback_ != nullptr) {
        writeCallback_(shared_from_this());
    }
}

void Channel::OnErrorable()
{
    if (errorCallback_ != nullptr) {
        errorCallback_(shared_from_this());
    }
}

void Channel::SetEvent(uint32_t event)
{
    epollEvt_ = event;
}

uint32_t Channel::GetEvent() const
{
    return epollEvt_;
}

SocketPtr Channel::GetSock() const
{
    return sock_;
}
