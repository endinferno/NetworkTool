#include "Poller/EventPoller.hpp"
#include "Logger.hpp"
#include "Poller/Epoller.hpp"

EventPoller::EventPoller()
    : poller_(std::make_unique<Epoller>())
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
    channels_.insert(tcpChan);
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
        if (event & Pollable::Event::EventErr) {
            tcpChan->OnErrorable();
        }
        if (event & Pollable::Event::EventOut) {
            tcpChan->OnWritable();
        }
        if (event & Pollable::Event::EventIn) {
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
