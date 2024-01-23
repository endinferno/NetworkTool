#pragma once

#include <poll.h>

#include "Poller/Pollable.hpp"

class Poller : public Pollable
{
public:
    Poller();
    TcpChannels PollEvent() override;
    void EventCtl(TcpChannel* tcpChan, int op, uint32_t event) override;
    ~Poller() override = default;

private:
    void AddEvent(TcpChannel* tcpChan, int fd, short event);
    void DelEvent(int fd, short event);
    void ModEvent(int fd, short event);

private:
    uint32_t pollEventMask;
    size_t eventSet;
    std::vector<struct pollfd> events_;
    std::unordered_map<int, TcpChannel*> channelMap;
};
