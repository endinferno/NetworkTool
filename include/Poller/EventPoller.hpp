#pragma once

#include <thread>
#include <unordered_set>

#include "Channel.hpp"
#include "Poller/Pollable.hpp"
#include "Utils/NonCopyable.hpp"

class EventPoller : public NonCopyable
{
public:
    EventPoller();
    void AddEvent(ChannelPtr& chan, uint32_t event);
    void ModEvent(ChannelPtr& chan, uint32_t event);
    void DelEvent(ChannelPtr& chan);
    void Run();
    ~EventPoller();

private:
    void HandleEvent(Channels& chans);
    void EpollThreadFn();

    PollablePtr poller_;
    bool isQuit_ = false;
    std::unique_ptr<std::thread> eventPollThread_;
    std::unordered_set<ChannelPtr> channels_;
};

using EventPollerPtr = std::shared_ptr<EventPoller>;
