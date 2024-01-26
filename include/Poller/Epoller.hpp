#pragma once

#include <vector>

#include <sys/epoll.h>

#include "Poller/Pollable.hpp"

class Epoller : public Pollable
{
public:
    Epoller();
    TcpChannels PollEvent() override;
    void EventCtl(TcpChannel* tcpChan, enum EventCtl op,
                  uint32_t event) override;
    ~Epoller() override;

private:
    int epollFd_;
    std::vector<struct epoll_event> events_;
};

using EpollerPtr = std::shared_ptr<Epoller>;
