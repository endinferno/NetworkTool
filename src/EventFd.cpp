#include <unistd.h>

#include "EventFd.hpp"

EventFd::EventFd()
    : eventFd_(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
{}

int EventFd::Write(eventfd_t event) const
{
    return ::eventfd_write(eventFd_, event);
}

int EventFd::Read(eventfd_t& event) const
{
    return ::eventfd_read(eventFd_, &event);
}

EventFd::~EventFd()
{
    ::close(eventFd_);
}
