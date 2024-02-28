#pragma once

#include <mutex>
#include <vector>

#include <sys/eventfd.h>

#include "EpollHandler.hpp"
#include "EventFd.hpp"
#include "Utils/NonCopyable.hpp"

class EventFdClient : public NonCopyable, public EpollHandler
{
public:
    using ReadCallback = std::function<void(eventfd_t&)>;
    using WriteCallback = std::function<void(void)>;

    explicit EventFdClient(EventPollerPtr& poller);
    void SetReadCallback(ReadCallback&& callback);
    size_t Create();
    int Write(size_t idx, const eventfd_t& event);
    ~EventFdClient() override = default;

private:
    void HandleErrorEvent(ChannelPtr&& chan) override;
    void HandleReadEvent(ChannelPtr&& chan) override;
    void HandleWriteEvent(ChannelPtr&& chan) override;

    std::vector<EventFdPtr> eventFds_;
    std::mutex createMtx_;
    ReadCallback readCallback_;
};
