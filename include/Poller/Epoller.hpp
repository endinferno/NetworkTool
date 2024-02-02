#pragma once

#include <vector>

#include <sys/epoll.h>

#include "Poller/Pollable.hpp"

class Epoller : public Pollable
{
public:
    Epoller();
    Channels PollEvent() override;
    void EventCtl(Channel* chan, enum EventCtl opera, uint32_t event) override;
    ~Epoller() override;

private:
    int epollFd_;
    std::vector<struct epoll_event> events_;
};

using EpollerPtr = std::shared_ptr<Epoller>;
