#pragma once

#include <memory>
#include <string>

#include "IPAddress.hpp"
#include "NonCopyable.hpp"

class Socket : public NonCopyable
{
public:
    explicit Socket(int sockFd);
    void SetReuseAddr() const;
    void SetReusePort() const;
    void SetNonBlock() const;
    [[nodiscard]] int GetSockOpt(int level, int optName) const;
    void Connect(const std::string& domainName, uint16_t port);
    ssize_t Write(const std::string& writeBuf);
    ssize_t Read(std::string& readBuf);
    [[nodiscard]] int GetFd() const;
    [[nodiscard]] int GetErrno() const;
    virtual ~Socket() = default;

private:
    void SetErrno(int err);
    IPAddress GetIPFromDomain(const std::string& domainName);

    int sockFd_;
    int savedErrno_;
};

using SocketPtr = std::shared_ptr<Socket>;
