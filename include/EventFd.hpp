#pragma once

#include <atomic>

#include <sys/eventfd.h>

#include "PosixFd.hpp"

class EventFd : public PosixFd
{
public:
    EventFd();
    [[nodiscard]] int Write(const eventfd_t& event) const;
    [[nodiscard]] int Read(eventfd_t& event) const;
    void SetWritable(bool isWritable);
    [[nodiscard]] bool IsWritable() const;
    ~EventFd() override;

private:
    std::atomic<bool> isWritable_;
};

using EventFdPtr = std::shared_ptr<EventFd>;
