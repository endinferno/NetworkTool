#include <stdexcept>

#include <sys/epoll.h>
#include <unistd.h>

#include "Epoller.hpp"
#include "Logger.hpp"

Epoller::Epoller()
    : epollFd_(epoll_create(EPOLL_MAX_EVENT))
    , events_(EPOLL_MAX_EVENT)
    , isQuit_(false)
{}

void Epoller::AddEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    channels_.insert(tcpChan);
    EpollEventCtl(tcpChan.get(), EPOLL_CTL_ADD, event);
}

void Epoller::ModEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    EpollEventCtl(tcpChan.get(), EPOLL_CTL_MOD, event);
}

void Epoller::DelEvent(TcpChannelPtr& tcpChan)
{
    channels_.erase(tcpChan);
    EpollEventCtl(tcpChan.get(), EPOLL_CTL_DEL, 0);
}

void Epoller::Run()
{
    epollThread_ = std::make_shared<std::thread>(&Epoller::EpollThreadFn, this);
}

void Epoller::EpollThreadFn()
{
    while (!isQuit_) {
        auto tcpChans = PollEvent();
        HandleEvent(tcpChans);
    }
}

Epoller::TcpChannels Epoller::PollEvent()
{
    std::vector<TcpChannel*> tcpChans;
    int eventCnt =
        ::epoll_wait(epollFd_, events_.data(), EPOLL_MAX_EVENT, EPOLL_TIMEOUT);
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

void Epoller::HandleEvent(TcpChannels& tcpChans)
{
    for (auto& tcpChan : tcpChans) {
        uint32_t event = tcpChan->GetEvent();
        DEBUG("Epoll event {:x}\n", event);
        if (event & EPOLLERR) {
            tcpChan->OnErrorable();
        }
        if (event & EPOLLOUT) {
            tcpChan->OnWritable();
        }
        if (event & EPOLLIN) {
            tcpChan->OnReadable();
        }
    }
}

void Epoller::EpollEventCtl(TcpChannel* tcpChan, int op, uint32_t event)
{
    struct epoll_event epollEvt;
    epollEvt.data.ptr = static_cast<void*>(tcpChan);
    epollEvt.events = event;

    int ret = ::epoll_ctl(epollFd_, op, tcpChan->GetSock()->GetFd(), &epollEvt);
    if (ret == -1) {
        throw std::runtime_error(
            fmt::format("Fail to {} epoll event {}\n", op, event));
    }
}

Epoller::~Epoller()
{
    isQuit_ = true;
    if (epollThread_->joinable()) {
        epollThread_->join();
    }
    ::close(epollFd_);
}
