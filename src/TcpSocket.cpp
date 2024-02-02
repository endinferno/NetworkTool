#include <stdexcept>

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "Logger.hpp"
#include "TcpSocket.hpp"

TcpSocket::TcpSocket()
    : sockFd_(::socket(AF_INET, SOCK_STREAM, 0))
{}

void TcpSocket::SetReuseAddr() const
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

void TcpSocket::SetReusePort() const
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

void TcpSocket::SetNonBlock() const
{
    int oldSockFlag = ::fcntl(sockFd_, F_GETFL, 0);
    ::fcntl(sockFd_, F_SETFL, oldSockFlag | O_NONBLOCK);
}

int TcpSocket::GetSockOpt(int level, int optName) const
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

void TcpSocket::Connect(const std::string& domainName, uint16_t port)
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

ssize_t TcpSocket::Write(const std::string& writeBuf)
{
    ssize_t writeBytes = ::write(sockFd_, writeBuf.data(), writeBuf.size());
    SetErrno(errno);
    return writeBytes;
}

ssize_t TcpSocket::Read(std::string& readBuf)
{
    ssize_t readBytes = ::read(sockFd_, readBuf.data(), readBuf.size());
    SetErrno(errno);
    return readBytes;
}

int TcpSocket::GetErrno() const
{
    return savedErrno_;
}

void TcpSocket::SetErrno(int err)
{
    savedErrno_ = err;
}

IPAddress TcpSocket::GetIPFromDomain(const std::string& domainName)
{
    struct hostent* host = ::gethostbyname(domainName.c_str());
    if (host == nullptr) {
        throw std::runtime_error("Fail to get IP from domain name " +
                                 domainName);
    }
    uint32_t netIp = *(reinterpret_cast<uint32_t*>(host->h_addr));
    return IPAddress(ntohl(netIp));
}

int TcpSocket::GetFd() const
{
    return sockFd_;
}

TcpSocket::~TcpSocket()
{
    ::close(sockFd_);
}
