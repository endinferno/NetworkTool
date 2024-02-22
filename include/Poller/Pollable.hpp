#pragma once

#include "Channel.hpp"

class Pollable
{
public:
    enum Event
    {
        EventIn = 0x01,
        EventPri = 0x02,
        EventOut = 0x04,
        EventErr = 0x08,
        EventRdhup = 0x2000,
        EventEt = 1U << 31
    };
    enum EventCtl
    {
        Add = 0x01,
        Del = 0x02,
        Mod = 0x03,
    };
    constexpr static uint32_t ERROR_EVENT = Pollable::Event::EventErr;
    constexpr static uint32_t READ_EVENT = Pollable::Event::EventIn |
                                           Pollable::Event::EventPri |
                                           Pollable::Event::EventRdhup;
    constexpr static uint32_t WRITE_EVENT = Pollable::Event::EventOut;
    Pollable() = default;
    virtual Channels PollEvent() = 0;
    virtual void EventCtl(Channel* chan, enum EventCtl opera,
                          uint32_t event) = 0;
    virtual ~Pollable() = default;

protected:
    constexpr static int EVENT_POLLER_MAX_EVENT = 32;
    constexpr static int EVENT_POLLER_TIMEOUT = 500;
};

using PollablePtr = std::unique_ptr<Pollable>;
