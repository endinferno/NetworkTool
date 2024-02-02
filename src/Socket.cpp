#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

#include "Logger.hpp"
#include "Socket.hpp"

Socket::Socket(int sockFd)
    : sockFd_(sockFd)
{}

void Socket::SetReuseAddr() const
{
    int opt = 1;
    int ret = ::setsockopt(sockFd_,
                           SOL_SOCKET,
                           SO_REUSEADDR,
                           reinterpret_cast<const void*>(&opt),
                           sizeof(opt));
    if (ret != 0) {
        throw std::runtime_error("Fail to reuse address");
    }
}

void Socket::SetReusePort() const
{
    int opt = 1;
    int ret = ::setsockopt(sockFd_,
                           SOL_SOCKET,
                           SO_REUSEPORT,
                           reinterpret_cast<const void*>(&opt),
                           sizeof(opt));
    if (ret != 0) {
        throw std::runtime_error("Fail to reuse address");
    }
}

void Socket::SetNonBlock() const
{
    int oldSockFlag = ::fcntl(sockFd_, F_GETFL, 0);
    ::fcntl(sockFd_, F_SETFL, oldSockFlag | O_NONBLOCK);
}

int Socket::GetSockOpt(int level, int optName) const
{
    int opt;
    socklen_t len = sizeof(int);
    int ret = ::getsockopt(
        sockFd_, level, optName, reinterpret_cast<void*>(&opt), &len);
    if (ret == -1) {
        throw std::runtime_error(
            fmt::format("Fail to get sock option {}\n", errno));
    }
    return opt;
}

void Socket::Connect(const std::string& domainName, uint16_t port)
{
    struct sockaddr_in serverAddr;

    auto domainIp = GetIPFromDomain(domainName);
    INFO("Connect to domain {} IP {} Port {}\n",
         domainName,
         domainIp.Stringify(),
         port);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = domainIp.GetNetIpAddr();

    int ret = ::connect(sockFd_,
                        reinterpret_cast<struct sockaddr*>(&serverAddr),
                        sizeof(serverAddr));
    if (ret == 0 || (ret < 0 && errno == EINPROGRESS)) {
        return;
    } else {
        throw std::runtime_error("Fail to connect server");
    }
}

ssize_t Socket::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = ::write(sockFd_, writeBuf.data(), writeBuf.size());
    SetErrno(errno);
    return writeBytes;
}

ssize_t Socket::Read(std::string& readBuf)
{
    ssize_t readBytes = ::read(sockFd_, readBuf.data(), readBuf.size());
    SetErrno(errno);
    return readBytes;
}

int Socket::GetErrno() const
{
    return savedErrno_;
}

void Socket::SetErrno(int err)
{
    savedErrno_ = err;
}

IPAddress Socket::GetIPFromDomain(const std::string& domainName)
{
    struct hostent* host = ::gethostbyname(domainName.c_str());
    if (host == nullptr) {
        throw std::runtime_error("Fail to get IP from domain name " +
                                 domainName);
    }
    uint32_t netIp = *(reinterpret_cast<uint32_t*>(host->h_addr));
    return IPAddress(ntohl(netIp));
}

int Socket::GetFd() const
{
    return sockFd_;
}
