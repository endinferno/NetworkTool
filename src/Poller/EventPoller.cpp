#include "Poller/EventPoller.hpp"
#include "Logger.hpp"
#if defined(USE_EPOLL)
#    include "Poller/Epoller.hpp"
#elif defined(USE_POLL)
#    include "Poller/Poller.hpp"
#elif defined(USE_SELECT)
#    include "Poller/Selector.hpp"
#endif

EventPoller::EventPoller()
#if defined(USE_EPOLL)
    : poller_(std::make_unique<Epoller>())
#elif defined(USE_POLL)
    : poller_(std::make_unique<Poller>())
#elif defined(USE_SELECT)
    : poller_(std::make_unique<Selector>())
#endif
{}

void EventPoller::AddEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    channels_.insert(tcpChan);
    poller_->EventCtl(tcpChan.get(), Pollable::EventCtl::Add, event);
}

void EventPoller::ModEvent(TcpChannelPtr& tcpChan, uint32_t event)
{
    poller_->EventCtl(tcpChan.get(), Pollable::EventCtl::Mod, event);
}

void EventPoller::DelEvent(TcpChannelPtr& tcpChan)
{
    channels_.erase(tcpChan);
    poller_->EventCtl(tcpChan.get(), Pollable::EventCtl::Del, 0);
}

void EventPoller::Run()
{
    eventPollThread_ =
        std::make_unique<std::thread>(&EventPoller::EpollThreadFn, this);
}

EventPoller::~EventPoller()
{
    isQuit_ = true;
    if (eventPollThread_->joinable()) {
        eventPollThread_->join();
    }
    eventPollThread_.reset();
    // destruct poller after the thread exiting
    poller_.reset();
}

void EventPoller::HandleEvent(TcpChannels& tcpChans)
{
    for (auto& tcpChan : tcpChans) {
        uint32_t event = tcpChan->GetEvent();
        DEBUG("Poll event {:x}\n", event);
        if ((event & Pollable::Event::EventErr) != 0) {
            tcpChan->OnErrorable();
        }
        if ((event & Pollable::Event::EventOut) != 0) {
            tcpChan->OnWritable();
        }
        if ((event & Pollable::Event::EventIn) != 0) {
            tcpChan->OnReadable();
        }
    }
}

void EventPoller::EpollThreadFn()
{
    while (!isQuit_) {
        auto tcpChans = poller_->PollEvent();
        HandleEvent(tcpChans);
    }
}
