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

void EventPoller::AddEvent(ChannelPtr& chan, uint32_t event)
{
    channels_.insert(chan);
    poller_->EventCtl(chan.get(), Pollable::EventCtl::Add, event);
}

void EventPoller::ModEvent(ChannelPtr& chan, uint32_t event)
{
    poller_->EventCtl(chan.get(), Pollable::EventCtl::Mod, event);
}

void EventPoller::DelEvent(ChannelPtr& chan)
{
    channels_.erase(chan);
    poller_->EventCtl(chan.get(), Pollable::EventCtl::Del, 0);
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

void EventPoller::HandleEvent(Channels& chans)
{
    for (auto& chan : chans) {
        uint32_t event = chan->GetEvent();
        DEBUG("Poll event {:x}\n", event);
        if ((event & Pollable::Event::EventErr) != 0) {
            chan->OnErrorable();
        }
        if ((event & Pollable::Event::EventOut) != 0) {
            chan->OnWritable();
        }
        if ((event & Pollable::Event::EventIn) != 0) {
            chan->OnReadable();
        }
    }
}

void EventPoller::EpollThreadFn()
{
    while (!isQuit_) {
        auto chans = poller_->PollEvent();
        HandleEvent(chans);
    }
}
