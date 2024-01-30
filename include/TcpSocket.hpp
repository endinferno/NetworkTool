#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <sys/socket.h>
#include <unistd.h>

#include "NonCopyable.hpp"

class TcpSocket : public NonCopyable
{
public:
    TcpSocket();
    void SetReuseAddr() const;
    void SetReusePort() const;
    void SetNonBlock() const;
    [[nodiscard]] int GetSockOpt(int level, int optName) const;
    void Connect(const std::string& domainName, uint16_t port);
    ssize_t Write(const std::string& writeBuf);
    ssize_t Read(std::string& readBuf);
    [[nodiscard]] int GetFd() const;
    [[nodiscard]] int GetErrno() const;
    ~TcpSocket();

private:
    void SetErrno(int err);
    uint32_t GetIPFromDomain(const std::string& domainName);

private:
    int sockFd_;
    int savedErrno_;
};

using TcpSocketPtr = std::shared_ptr<TcpSocket>;
