#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <sys/socket.h>
#include <unistd.h>

#include "NonCopyable.hpp"

class TcpSocket : public NonCopyable
{
public:
    TcpSocket();
    void SetReuseAddr();
    void SetReusePort();
    void SetNonBlock();
    int GetSockOpt(int level, int optName);
    void Connect(const std::string& domainName, uint16_t port);
    ssize_t Write(const std::vector<char>& writeBuf);
    ssize_t Read(std::vector<char>& readBuf);
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
