#include <csignal>

#include "Signal.hpp"

std::atomic<bool> Signal::sigTrigger = false;

Signal::Signal()
{
    ::signal(SIGTERM, Signal::HandleSignal);
    ::signal(SIGINT, Signal::HandleSignal);
}

void Signal::HandleSignal([[maybe_unused]] int signum)
{
    sigTrigger.store(true);
}

bool Signal::IsSignalTrigger()
{
    return sigTrigger.load();
}
