#pragma once

#include <functional>
#include <memory>

#include "TcpSocket.hpp"

class TcpChannel : public std::enable_shared_from_this<TcpChannel>
{
public:
    using EventCallback = std::function<void(std::shared_ptr<TcpChannel>)>;

    explicit TcpChannel(const std::shared_ptr<TcpSocket>& tcpSock);
    void SetReadCallback(EventCallback callback);
    void SetWriteCallback(EventCallback callback);
    void SetErrorCallback(EventCallback callback);
    void OnReadable();
    void OnWritable();
    void OnErrorable();
    void SetEvent(uint32_t event);
    [[nodiscard]] uint32_t GetEvent() const;
    [[nodiscard]] std::shared_ptr<TcpSocket> GetSock() const;
    ~TcpChannel() = default;

private:
    std::shared_ptr<TcpSocket> tcpSock_;
    uint32_t epollEvt_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};
