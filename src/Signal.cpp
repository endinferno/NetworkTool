#include <csignal>

#include "Signal.hpp"

std::atomic<bool> Signal::sigTrigger = false;

Signal::Signal()
{
    ::signal(SIGTERM, Signal::HandleSignal);
    ::signal(SIGINT, Signal::HandleSignal);
}

void Signal::HandleSignal(int signum)
{
    sigTrigger.store(true);
}

bool Signal::IsSignalTrigger() const
{
    return sigTrigger.load();
}
