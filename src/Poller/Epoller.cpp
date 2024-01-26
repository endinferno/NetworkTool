#include <stdexcept>

#include <sys/epoll.h>
#include <unistd.h>

#include "Logger.hpp"
#include "Poller/Epoller.hpp"

Epoller::Epoller()
    : epollFd_(epoll_create(EVENT_POLLER_MAX_EVENT))
    , events_(EVENT_POLLER_MAX_EVENT)
{}

Epoller::~Epoller()
{
    ::close(epollFd_);
}

TcpChannels Epoller::PollEvent()
{
    std::vector<TcpChannel*> tcpChans;
    int eventCnt = ::epoll_wait(
        epollFd_, events_.data(), EVENT_POLLER_MAX_EVENT, EVENT_POLLER_TIMEOUT);
    DEBUG("eventCnt {}\n", eventCnt);
    if (eventCnt == -1) {
        throw std::runtime_error(
            fmt::format("Fail to epoll_wait {} {}\n", eventCnt, errno));
    } else if (eventCnt == 0) {
        // Timeout rings
        return tcpChans;
    }
    for (int i = 0; i < eventCnt; i++) {
        auto tcpChan = static_cast<TcpChannel*>(events_[i].data.ptr);
        tcpChan->SetEvent(events_[i].events);
        tcpChans.emplace_back(tcpChan);
    }
    return tcpChans;
}

void Epoller::EventCtl(TcpChannel* tcpChan, enum EventCtl op, uint32_t event)
{
    struct epoll_event epollEvt;
    epollEvt.data.ptr = static_cast<void*>(tcpChan);
    epollEvt.events = event;

    int ret = ::epoll_ctl(
        epollFd_, static_cast<int>(op), tcpChan->GetSock()->GetFd(), &epollEvt);
    if (ret == -1) {
        throw std::runtime_error(fmt::format(
            "Fail to {} epoll event {}\n", static_cast<int>(op), event));
    }
}
