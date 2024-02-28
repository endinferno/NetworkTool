#include <type_traits>
#include <unistd.h>

#include "EventFd.hpp"

EventFd::EventFd()
    : PosixFd(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC))
{}

int EventFd::Write(const eventfd_t& event) const
{
    return ::eventfd_write(GetFd(), event);
}

int EventFd::Read(eventfd_t& event) const
{
    return ::eventfd_read(GetFd(), &event);
}

void EventFd::SetWritable(bool isWritable)
{
    isWritable_ = isWritable;
}

bool EventFd::IsWritable() const
{
    return isWritable_;
}

EventFd::~EventFd()
{
    ::close(GetFd());
}
