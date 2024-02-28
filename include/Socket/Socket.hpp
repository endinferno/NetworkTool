#pragma once

#include <memory>
#include <string>

#include "PosixFd.hpp"
#include "Utils/IPAddress.hpp"

class Socket;

using SocketPtr = std::shared_ptr<Socket>;

class Socket : public PosixFd
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
    [[nodiscard]] int GetErrno() const;
    ssize_t Recvfrom(std::string& readBuf,
                     struct sockaddr_in& clientAddr) const;
    virtual void Bind(const IPAddress& localIp, const uint16_t& localPort) = 0;
    virtual void Listen() = 0;
    virtual SocketPtr Accept(struct sockaddr_in& clientAddr) = 0;
    ~Socket() override = default;

protected:
    void SetErrno(int err);

private:
    int savedErrno_;
};
