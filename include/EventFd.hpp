#pragma once

#include <sys/eventfd.h>

class EventFd
{
public:
    EventFd();
    [[nodiscard]] int Write(eventfd_t event) const;
    [[nodiscard]] int Read(eventfd_t& event) const;
    ~EventFd();

private:
    int eventFd_;
};
