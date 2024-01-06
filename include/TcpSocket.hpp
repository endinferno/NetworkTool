#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <sys/socket.h>
#include <unistd.h>

#include "NonCopyable.hpp"
#include "TcpConnection.hpp"

class TcpConnection;

class TcpSocket : public NonCopyable,
                  public std::enable_shared_from_this<TcpSocket>
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
    ~TcpSocket();

private:
    uint32_t GetIPFromDomain(const std::string& domainName);

private:
    int sockFd_;
};
