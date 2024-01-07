#pragma once

#include "Epoller.hpp"
#include "TcpChannel.hpp"

class EpollHandler
{
public:
    explicit EpollHandler(std::shared_ptr<Epoller>& epoller)
        : epoller_(epoller)
    {}
    virtual void HandleErrorEvent(std::shared_ptr<TcpChannel> tcpChan) = 0;
    virtual void HandleReadEvent(std::shared_ptr<TcpChannel> tcpChan) = 0;
    virtual void HandleWriteEvent(std::shared_ptr<TcpChannel> tcpChan) = 0;
    ~EpollHandler() = default;

protected:
    std::shared_ptr<Epoller> epoller_;
};
