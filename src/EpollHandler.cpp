#include "EpollHandler.hpp"

EpollHandler::EpollHandler(EventPollerPtr& poller)
    : poller_(poller)
{}

void EpollHandler::AddEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    poller_->AddEvent(tcpChan, event);
}

void EpollHandler::ModEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    poller_->ModEvent(tcpChan, event);
}

void EpollHandler::DelEvent(TcpChannelPtr& tcpChan)
{
    poller_->DelEvent(tcpChan);
}
