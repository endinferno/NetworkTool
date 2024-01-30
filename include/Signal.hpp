#pragma once

#include <atomic>

class Signal
{
public:
    Signal();
    static void HandleSignal(int signum);
    static bool IsSignalTrigger();
    ~Signal() = default;

private:
    static std::atomic<bool> sigTrigger;
};
