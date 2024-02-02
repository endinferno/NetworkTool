#pragma once

#include <sys/select.h>

#include "Poller/Pollable.hpp"

class Selector : public Pollable
{
public:
    Selector() = default;
    Channels PollEvent() override;
    void EventCtl(Channel* chan, enum EventCtl opera, uint32_t event) override;
    ~Selector() override = default;

private:
    void FillFds(fd_set* readFds, fd_set* writeFds, fd_set* exceptFds,
                 int& maxFd);
    void EmptyFds(fd_set* readFds, fd_set* writeFds, fd_set* exceptFds);
    void FillIntervalTime(struct timeval& intervalTime);

    std::unordered_map<int, std::pair<uint32_t, Channel*>> eventChanMap_;
};
