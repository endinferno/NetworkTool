#pragma once

#include "Epoller.hpp"
#include "TcpChannel.hpp"

class EpollHandler
{
public:
    explicit EpollHandler(EpollerPtr& epoller)
        : epoller_(epoller)
    {}
    virtual void HandleErrorEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleReadEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleWriteEvent(TcpChannelPtr tcpChan) = 0;
    ~EpollHandler() = default;

protected:
    EpollerPtr epoller_;
};
