#pragma once

#include <functional>

#include "TcpSocket.hpp"

class TcpChannel;

using TcpChannelPtr = std::shared_ptr<TcpChannel>;
using TcpChannels = std::vector<TcpChannel*>;

class TcpChannel : public std::enable_shared_from_this<TcpChannel>
{
public:
    using EventCallback = std::function<void(TcpChannelPtr)>;

    explicit TcpChannel(TcpSocketPtr& tcpSock);
    void SetReadCallback(EventCallback callback);
    void SetWriteCallback(EventCallback callback);
    void SetErrorCallback(EventCallback callback);
    void OnReadable();
    void OnWritable();
    void OnErrorable();
    void SetEvent(uint32_t event);
    [[nodiscard]] uint32_t GetEvent() const;
    [[nodiscard]] TcpSocketPtr GetSock() const;
    ~TcpChannel() = default;

private:
    TcpSocketPtr tcpSock_;
    uint32_t epollEvt_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};
