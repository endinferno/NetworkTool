#pragma once

#include <memory>
#include <string>

#include "Utils/IPAddress.hpp"
#include "Utils/NonCopyable.hpp"

class Socket;

using SocketPtr = std::shared_ptr<Socket>;

class Socket : public NonCopyable
{
public:
    enum SocketType
    {
        TCP,
        UDP
    };
    explicit Socket(int sockFd);
    void SetReuseAddr() const;
    void SetReusePort() const;
    void SetNonBlock() const;
    [[nodiscard]] int GetSockOpt(int level, int optName) const;
    void Connect(const IPAddress& serverIp, const uint16_t& serverPort) const;
    void Connect(struct sockaddr_in& serverAddr) const;
    ssize_t Write(const std::string& writeBuf);
    ssize_t Read(std::string& readBuf);
    [[nodiscard]] int GetFd() const;
    [[nodiscard]] int GetErrno() const;
    ssize_t Recvfrom(std::string& readBuf,
                     struct sockaddr_in& clientAddr) const;
    virtual void Bind(const IPAddress& localIp, const uint16_t& localPort) = 0;
    virtual void Listen() = 0;
    virtual SocketPtr Accept(struct sockaddr_in& clientAddr) = 0;
    virtual ~Socket() = default;

protected:
    void SetErrno(int err);

private:
    int sockFd_;
    int savedErrno_;
};
