#pragma once

#include <memory>
#include <thread>
#include <vector>

#include <sys/epoll.h>

#include "NonCopyable.hpp"
#include "TcpSocket.hpp"

class Epoller : public NonCopyable
{
public:
    Epoller();
    void AddEvent(std::shared_ptr<TcpConnection>& conn, uint32_t event);
    void ModEvent(std::shared_ptr<TcpConnection>& conn, uint32_t event);
    void DelEvent(std::shared_ptr<TcpConnection>& conn);
    void Run();
    ~Epoller();

private:
    void EpollThreadFn();
    std::vector<TcpConnection*> PollEvent();
    void HandleEvent(std::vector<TcpConnection*>& conns);
    void EpollEventCtl(TcpConnection* conn, int op, uint32_t event);

private:
    constexpr static int EPOLL_MAX_EVENT = 32;
    constexpr static int EPOLL_TIMEOUT = 500;

    int epollFd_;
    std::vector<struct epoll_event> events_;
    std::shared_ptr<std::thread> epollThread_;
    bool isQuit_;
};
