#include "Poller/EventPoller.hpp"
#include "Poller/Epoller.hpp"
#include "Utils/Logger.hpp"

EventPoller::EventPoller()
    : poller_(std::make_unique<Epoller>())
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
        if ((event & Pollable::ERROR_EVENT) != 0) {
            chan->OnErrorable();
        }
        if ((event & Pollable::READ_EVENT) != 0) {
            chan->OnReadable();
        }
        if ((event & Pollable::WRITE_EVENT) != 0) {
            chan->OnWritable();
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
