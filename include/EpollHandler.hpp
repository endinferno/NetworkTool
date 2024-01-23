#pragma once

#include "Poller/EventPoller.hpp"
#include "TcpChannel.hpp"

class EpollHandler
{
public:
    explicit EpollHandler(EventPollerPtr& poller)
        : poller_(poller)
    {}
    virtual void HandleErrorEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleReadEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleWriteEvent(TcpChannelPtr tcpChan) = 0;
    virtual ~EpollHandler() = default;

protected:
    EventPollerPtr poller_;
};
