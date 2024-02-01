#include <stdexcept>

#include "Logger.hpp"
#include "Poller/Poller.hpp"

Poller::Poller()
    : eventSet_(0)
    , events_(EVENT_POLLER_MAX_EVENT)
{
    for (auto& pollEvt : events_) {
        pollEvt.fd = -1;
        pollEvt.events = 0;
        pollEvt.revents = 0;
    }
}

TcpChannels Poller::PollEvent()
{
    std::vector<TcpChannel*> tcpChans;
    int eventCnt = ::poll(events_.data(), eventSet_, EVENT_POLLER_TIMEOUT);
    DEBUG("eventCnt {}\n", eventCnt);
    if (eventCnt == -1) {
        throw std::runtime_error(
            fmt::format("Fail to epoll_wait {} {}\n", eventCnt, errno));
    } else if (eventCnt == 0) {
        // Timeout rings
        return tcpChans;
    }
    for (size_t i = 0; i < eventSet_; i++) {
        if (events_[i].revents == 0) {
            continue;
        }
        auto tcpChan = channelMap_[events_[i].fd];
        tcpChan->SetEvent(events_[i].revents);
        events_[i].revents = 0;
        tcpChans.emplace_back(tcpChan);
    }
    return tcpChans;
}

void Poller::EventCtl(TcpChannel* tcpChan, enum EventCtl opera, uint32_t event)
{
    int sockFd = tcpChan->GetSock()->GetFd();
    switch (opera) {
    case Pollable::EventCtl::Add:
    {
        AddEvent(tcpChan, sockFd, event);
        break;
    }
    case Pollable::EventCtl::Del:
    {
        DelEvent(sockFd);
        break;
    }
    case Pollable::EventCtl::Mod:
    {
        ModEvent(sockFd, event);
        break;
    }
    default:
        throw std::runtime_error(
            fmt::format("No such operation {}\n", static_cast<int>(opera)));
    }
}

void Poller::AddEvent(TcpChannel* tcpChan, int sockFd, uint32_t event)
{
    if (eventSet_ == events_.size()) {
        throw std::runtime_error("Poll list is full\n");
    }
    channelMap_[sockFd] = tcpChan;
    events_[eventSet_].fd = sockFd;
    events_[eventSet_].events = static_cast<int16_t>(event);
    events_[eventSet_].revents = 0;
    eventSet_++;
}

void Poller::DelEvent(int sockFd)
{
    if (!channelMap_.count(sockFd)) {
        return;
    }
    channelMap_.erase(sockFd);
    auto endIt =
        events_.begin() +
        static_cast<std::vector<struct pollfd>::difference_type>(eventSet_) - 1;
    for (auto& poll : events_) {
        if (poll.fd == sockFd) {
            poll.fd = endIt->fd;
            poll.events = endIt->events;
            poll.revents = 0;
            endIt->fd = -1;
            endIt->events = 0;
            endIt->revents = 0;
            break;
        }
    }
    eventSet_--;
}

void Poller::ModEvent(int sockFd, uint32_t event)
{
    for (auto& poll : events_) {
        if (poll.fd == sockFd) {
            poll.events = static_cast<int16_t>(event);
            poll.revents = 0;
            break;
        }
    }
}
