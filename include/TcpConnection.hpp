#pragma once

#include <functional>
#include <memory>

#include "TcpSocket.hpp"

class TcpSocket;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    using EventCallback = std::function<void(std::shared_ptr<TcpConnection>)>;

    explicit TcpConnection(const std::shared_ptr<TcpSocket>& sock);
    void SetReadCallback(EventCallback callback);
    void SetWriteCallback(EventCallback callback);
    void SetErrorCallback(EventCallback callback);
    void OnReadable();
    void OnWritable();
    void OnErrorable();
    void SetEvent(uint32_t event);
    [[nodiscard]] uint32_t GetEvent() const;
    [[nodiscard]] std::shared_ptr<TcpSocket> GetSock() const;
    ~TcpConnection() = default;

protected:
    std::shared_ptr<TcpSocket> tcpSock_;
    uint32_t epollEvt_;
    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
};
