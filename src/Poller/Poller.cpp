#include <stdexcept>

#include "Logger.hpp"
#include "Poller/Poller.hpp"

Poller::Poller()
    : eventSet(0)
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
    int eventCnt = ::poll(events_.data(), eventSet, EVENT_POLLER_TIMEOUT);
    DEBUG("eventCnt {}\n", eventCnt);
    if (eventCnt == -1) {
        throw std::runtime_error(
            fmt::format("Fail to epoll_wait {} {}\n", eventCnt, errno));
    } else if (eventCnt == 0) {
        // Timeout rings
        return tcpChans;
    }
    for (size_t i = 0; i < eventSet; i++) {
        if (events_[i].revents == 0) {
            continue;
        }
        auto tcpChan = channelMap[events_[i].fd];
        tcpChan->SetEvent(events_[i].revents);
        events_[i].revents = 0;
        tcpChans.emplace_back(tcpChan);
    }
    return tcpChans;
}

void Poller::EventCtl(TcpChannel* tcpChan, int op, uint32_t event)
{
    int fd = tcpChan->GetSock()->GetFd();
    switch (op) {
    case Pollable::EventCtl::Add:
    {
        AddEvent(tcpChan, fd, event);
        break;
    }
    case Pollable::EventCtl::Del:
    {
        DelEvent(fd);
        break;
    }
    case Pollable::EventCtl::Mod:
    {
        ModEvent(fd, event);
        break;
    }
    default: throw std::runtime_error("");
    }
}

void Poller::AddEvent(TcpChannel* tcpChan, int fd, uint32_t event)
{
    if (eventSet == events_.size()) {
        throw std::runtime_error("Poll list is full\n");
    }
    channelMap[fd] = tcpChan;
    events_[eventSet].fd = fd;
    events_[eventSet].events = static_cast<short>(event);
    events_[eventSet].revents = 0;
    eventSet++;
}

void Poller::DelEvent(int fd)
{
    if (!channelMap.count(fd)) {
        return;
    }
    channelMap.erase(fd);
    auto endIt =
        events_.begin() +
        static_cast<std::vector<struct pollfd>::difference_type>(eventSet) - 1;
    for (auto& poll : events_) {
        if (poll.fd == fd) {
            poll.fd = endIt->fd;
            poll.events = endIt->events;
            poll.revents = 0;
            endIt->fd = -1;
            endIt->events = 0;
            endIt->revents = 0;
            break;
        }
    }
    eventSet--;
}

void Poller::ModEvent(int fd, uint32_t event)
{
    for (auto& poll : events_) {
        if (poll.fd == fd) {
            poll.events = static_cast<short>(event);
            poll.revents = 0;
            break;
        }
    }
}
