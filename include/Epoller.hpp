#pragma once

#include <memory>
#include <thread>
#include <unordered_set>
#include <vector>

#include <sys/epoll.h>

#include "NonCopyable.hpp"
#include "TcpChannel.hpp"

class Epoller : public NonCopyable
{
public:
    using TcpChannels = std::vector<TcpChannel*>;

    Epoller();
    void AddEvent(std::shared_ptr<TcpChannel>& tcpChan, uint32_t event);
    void ModEvent(std::shared_ptr<TcpChannel>& tcpChan, uint32_t event);
    void DelEvent(std::shared_ptr<TcpChannel>& tcpChan);
    void Run();
    ~Epoller();

private:
    void EpollThreadFn();
    TcpChannels PollEvent();
    void HandleEvent(TcpChannels& tcpChans);
    void EpollEventCtl(TcpChannel* tcpChan, int op, uint32_t event);

private:
    constexpr static int EPOLL_MAX_EVENT = 32;
    constexpr static int EPOLL_TIMEOUT = 500;

    int epollFd_;
    std::vector<struct epoll_event> events_;
    std::shared_ptr<std::thread> epollThread_;
    std::unordered_set<std::shared_ptr<TcpChannel>> channels_;
    bool isQuit_;
};
