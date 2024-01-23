#pragma once

#include <thread>
#include <unordered_set>

#include "NonCopyable.hpp"
#include "Poller/Pollable.hpp"
#include "TcpChannel.hpp"

class EventPoller : public NonCopyable
{
public:
    EventPoller();
    void AddEvent(TcpChannelPtr& tcpChan, uint32_t event);
    void ModEvent(TcpChannelPtr& tcpChan, uint32_t event);
    void DelEvent(TcpChannelPtr& tcpChan);
    void Run();
    ~EventPoller();

private:
    void HandleEvent(TcpChannels& tcpChans);
    void EpollThreadFn();

private:
    std::unique_ptr<Pollable> poller_;
    bool isQuit_ = false;
    std::unique_ptr<std::thread> eventPollThread_;
    std::unordered_set<TcpChannelPtr> channels_;
};

using EventPollerPtr = std::shared_ptr<EventPoller>;
