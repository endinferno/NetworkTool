#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdexcept>

#include "Socket/Socket.hpp"
#include "Utils/Logger.hpp"

Socket::Socket(int sockFd)
    : PosixFd(sockFd)
{}

void Socket::SetReuseAddr() const
{
    int opt = 1;
    int ret = ::setsockopt(GetFd(),
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
    int ret = ::setsockopt(GetFd(),
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
    int oldSockFlag = ::fcntl(GetFd(), F_GETFL, 0);
    ::fcntl(GetFd(), F_SETFL, oldSockFlag | O_NONBLOCK);
}

int Socket::GetSockOpt(int level, int optName) const
{
    int opt;
    socklen_t len = sizeof(int);
    int ret = ::getsockopt(
        GetFd(), level, optName, reinterpret_cast<void*>(&opt), &len);
    if (ret == -1) {
        throw std::runtime_error(
            fmt::format("Fail to get sock option {}\n", errno));
    }
    return opt;
}

void Socket::Connect(const IPAddress& serverIp,
                     const uint16_t& serverPort) const
{
    struct sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = serverIp.GetNetIpAddr();

    Connect(serverAddr);
}

void Socket::Connect(struct sockaddr_in& serverAddr) const
{
    INFO("Connect to IP {} Port {}\n",
         inet_ntoa(serverAddr.sin_addr),
         ntohs(serverAddr.sin_port));

    int ret = ::connect(GetFd(),
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
    ssize_t writeBytes = ::write(GetFd(), writeBuf.data(), writeBuf.size());
    SetErrno(errno);
    return writeBytes;
}

ssize_t Socket::Read(std::string& readBuf)
{
    ssize_t readBytes = ::read(GetFd(), readBuf.data(), readBuf.size());
    SetErrno(errno);
    return readBytes;
}

ssize_t Socket::Recvfrom(std::string& readBuf,
                         struct sockaddr_in& clientAddr) const
{
    socklen_t clientAddrLen = sizeof(clientAddr);

    return ::recvfrom(GetFd(),
                      readBuf.data(),
                      readBuf.size(),
                      0,
                      reinterpret_cast<struct sockaddr*>(&clientAddr),
                      &clientAddrLen);
}

int Socket::GetErrno() const
{
    return savedErrno_;
}

void Socket::SetErrno(int err)
{
    savedErrno_ = err;
}
