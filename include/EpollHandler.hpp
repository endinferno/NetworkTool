#pragma once

#include "Poller/EventPoller.hpp"
#include "TcpChannel.hpp"

class EpollHandler
{
public:
    explicit EpollHandler(EventPollerPtr& poller);
    virtual void HandleErrorEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleReadEvent(TcpChannelPtr tcpChan) = 0;
    virtual void HandleWriteEvent(TcpChannelPtr tcpChan) = 0;
    virtual ~EpollHandler() = default;

protected:
    void AddEvent(TcpChannelPtr& tcpChan, uint32_t event);
    void ModEvent(TcpChannelPtr& tcpChan, uint32_t event);
    void DelEvent(TcpChannelPtr& tcpChan);

private:
    EventPollerPtr poller_;
};
