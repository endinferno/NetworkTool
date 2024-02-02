#include "EpollHandler.hpp"

EpollHandler::EpollHandler(EventPollerPtr& poller)
    : poller_(poller)
{}

void EpollHandler::AddEvent(ChannelPtr& chan, uint32_t event)
{
    poller_->AddEvent(chan, event);
}

void EpollHandler::ModEvent(ChannelPtr& chan, uint32_t event)
{
    poller_->ModEvent(chan, event);
}

void EpollHandler::DelEvent(ChannelPtr& chan)
{
    poller_->DelEvent(chan);
}
