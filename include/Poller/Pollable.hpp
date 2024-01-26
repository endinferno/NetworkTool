#pragma once

#include "TcpChannel.hpp"

class Pollable
{
public:
    enum Event
    {
        EventIn = 0x01,
        EventOut = 0x04,
        EventErr = 0x08,
        EventEt = 1u << 31
    };
    enum EventCtl
    {
        Add = 0x01,
        Del = 0x02,
        Mod = 0x03,
    };
    Pollable() = default;
    virtual TcpChannels PollEvent() = 0;
    virtual void EventCtl(TcpChannel* tcpChan, enum EventCtl op,
                          uint32_t event) = 0;
    virtual ~Pollable() = default;

protected:
    constexpr static int EVENT_POLLER_MAX_EVENT = 32;
    constexpr static int EVENT_POLLER_TIMEOUT = 500;
};
