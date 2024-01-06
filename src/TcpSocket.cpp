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

void TcpSocket::SetReuseAddr()
{
    int opt = 1;
    int ret = ::setsockopt(
        sockFd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&opt, sizeof(opt));
    if (ret != 0) {
        throw std::runtime_error("Fail to reuse address");
    }
}

void TcpSocket::SetReusePort()
{
    int opt = 1;
    int ret = ::setsockopt(
        sockFd_, SOL_SOCKET, SO_REUSEPORT, (const void*)&opt, sizeof(opt));
    if (ret != 0) {
        throw std::runtime_error("Fail to reuse address");
    }
}

void TcpSocket::SetNonBlock()
{
    int oldSockFlag = ::fcntl(sockFd_, F_GETFL, 0);
    ::fcntl(sockFd_, F_SETFL, oldSockFlag | O_NONBLOCK);
}

int TcpSocket::GetSockOpt(int level, int optName)
{
    int opt;
    socklen_t len = sizeof(int);
    int ret = ::getsockopt(sockFd_, level, optName, (void*)&opt, &len);
    if (ret == -1) {
        throw std::runtime_error(
            fmt::format("Fail to get sock option {}\n", errno));
    }
    return opt;
}

void TcpSocket::Connect(const std::string& domainName, uint16_t port)
{
    struct sockaddr_in serverAddr;

    uint32_t ip = GetIPFromDomain(domainName);
    INFO("Connect to domain {} IP {}.{}.{}.{} Port {}\n",
         domainName,
         ip & 0xFF,
         (ip >> 8) & 0xFF,
         (ip >> 16) & 0xFF,
         (ip >> 24) & 0xFF,
         port);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = ip;

    int ret =
        ::connect(sockFd_, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret == 0 || (ret < 0 && errno == EINPROGRESS)) {
        return;
    } else {
        throw std::runtime_error("Fail to connect server");
    }
}

ssize_t TcpSocket::Write(const std::vector<char>& writeBuf)
{
    return ::write(sockFd_, (const void*)writeBuf.data(), writeBuf.size());
}

ssize_t TcpSocket::Read(std::vector<char>& readBuf)
{
    return ::read(sockFd_, readBuf.data(), readBuf.size());
}

uint32_t TcpSocket::GetIPFromDomain(const std::string& domainName)
{
    struct hostent* host = ::gethostbyname(domainName.c_str());
    if (host == nullptr) {
        throw std::runtime_error("Fail to get IP from domain name " +
                                 domainName);
    }
    return *((uint32_t*)host->h_addr);
}

int TcpSocket::GetFd() const
{
    return sockFd_;
}

TcpSocket::~TcpSocket()
{
    ::close(sockFd_);
}
