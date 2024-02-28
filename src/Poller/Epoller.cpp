#include <sys/epoll.h>
#include <unistd.h>

#include "Poller/Epoller.hpp"
#include "Utils/Logger.hpp"

Epoller::Epoller()
    : epollFd_(epoll_create(EVENT_POLLER_MAX_EVENT))
    , events_(EVENT_POLLER_MAX_EVENT)
{}

Epoller::~Epoller()
{
    ::close(epollFd_);
}

Channels Epoller::PollEvent()
{
    std::vector<Channel*> chans;
    int eventCnt = ::epoll_wait(
        epollFd_, events_.data(), EVENT_POLLER_MAX_EVENT, EVENT_POLLER_TIMEOUT);
    DEBUG("eventCnt {}\n", eventCnt);
    if (eventCnt == -1) {
        throw std::runtime_error(
            fmt::format("Fail to epoll_wait {} {}\n", eventCnt, errno));
    } else if (eventCnt == 0) {
        // Timeout rings
        return chans;
    }
    for (int i = 0; i < eventCnt; i++) {
        auto chan = static_cast<Channel*>(events_[i].data.ptr);
        chan->SetEvent(events_[i].events);
        chans.emplace_back(chan);
    }
    return chans;
}

void Epoller::EventCtl(Channel* chan, enum EventCtl opera, uint32_t event)
{
    struct epoll_event epollEvt;
    epollEvt.data.ptr = static_cast<void*>(chan);
    epollEvt.events = event;

    int ret = ::epoll_ctl(
        epollFd_, static_cast<int>(opera), chan->GetFd()->GetFd(), &epollEvt);
    if (ret == -1) {
        throw std::runtime_error(fmt::format(
            "Fail to {} epoll event {}\n", static_cast<int>(opera), event));
    }
}
