#pragma once

#include <memory>
#include <string>

#include "Utils/IPAddress.hpp"
#include "Utils/NonCopyable.hpp"

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
    ssize_t Write(const std::string& writeBuf);
    ssize_t Read(std::string& readBuf);
    [[nodiscard]] int GetFd() const;
    [[nodiscard]] int GetErrno() const;
    virtual ~Socket() = default;

private:
    void SetErrno(int err);

    int sockFd_;
    int savedErrno_;
};

using SocketPtr = std::shared_ptr<Socket>;
