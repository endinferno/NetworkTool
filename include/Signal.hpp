#pragma once

#include <atomic>

class Signal
{
public:
    Signal();
    static void HandleSignal(int signum);
    [[nodiscard]] bool IsSignalTrigger() const;
    ~Signal() = default;

private:
    static std::atomic<bool> sigTrigger;
};
