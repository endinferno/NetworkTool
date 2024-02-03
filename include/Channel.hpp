#pragma once

#include <functional>

#include "Socket.hpp"

class Channel;

using ChannelPtr = std::shared_ptr<Channel>;
using Channels = std::vector<Channel*>;

class Channel : public std::enable_shared_from_this<Channel>
{
public:
    using EventCallback = std::function<void(ChannelPtr)>;

    explicit Channel(SocketPtr& sock);
    void SetReadCallback(EventCallback callback);
    void SetWriteCallback(EventCallback callback);
    void SetErrorCallback(EventCallback callback);
    void OnReadable();
    void OnWritable();
    void OnErrorable();
    void SetEvent(uint32_t event);
    [[nodiscard]] uint32_t GetEvent() const;
    [[nodiscard]] SocketPtr GetSock() const;
    ~Channel() = default;

private:
    SocketPtr sock_;
    uint32_t epollEvt_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};