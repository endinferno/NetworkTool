#pragma once

#include <poll.h>

#include "Poller/Pollable.hpp"

class Poller : public Pollable
{
public:
    Poller();
    TcpChannels PollEvent() override;
    void EventCtl(TcpChannel* tcpChan, enum EventCtl opera,
                  uint32_t event) override;
    ~Poller() override = default;

private:
    void AddEvent(TcpChannel* tcpChan, int sockFd, uint32_t event);
    void DelEvent(int sockFd);
    void ModEvent(int sockFd, uint32_t event);

    size_t eventSet_;
    std::vector<struct pollfd> events_;
    std::unordered_map<int, TcpChannel*> channelMap_;
};
