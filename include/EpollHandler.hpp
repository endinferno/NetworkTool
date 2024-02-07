#pragma once

#include "Channel.hpp"
#include "Poller/EventPoller.hpp"

class EpollHandler
{
public:
    explicit EpollHandler(EventPollerPtr& poller);
    virtual void HandleErrorEvent(ChannelPtr&& chan) = 0;
    virtual void HandleReadEvent(ChannelPtr&& chan) = 0;
    virtual void HandleWriteEvent(ChannelPtr&& chan) = 0;
    void AddEvent(ChannelPtr& chan, uint32_t event);
    void ModEvent(ChannelPtr& chan, uint32_t event);
    void DelEvent(ChannelPtr& chan);
    virtual ~EpollHandler() = default;

private:
    EventPollerPtr poller_;
};
