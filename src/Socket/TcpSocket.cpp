#include <sys/socket.h>
#include <unistd.h>

#include "Socket/TcpSocket.hpp"

TcpSocket::TcpSocket()
    : Socket(::socket(AF_INET, SOCK_STREAM, 0))
{}

TcpSocket::TcpSocket(int sockFd)
    : Socket(sockFd)
{}

void TcpSocket::Bind(const IPAddress& localIp, const uint16_t& localPort)
{
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(localPort);
    serverAddr.sin_addr.s_addr = localIp.GetNetIpAddr();

    int ret = ::bind(GetFd(),
                     reinterpret_cast<struct sockaddr*>(&serverAddr),
                     sizeof(struct sockaddr));
    if (ret != 0) {
        throw std::runtime_error(
            fmt::format("Fail to bind socket to {}:{}\n", localIp, localPort));
    }
}

void TcpSocket::Listen()
{
    int ret = ::listen(GetFd(), SOMAXCONN);
    if (ret != 0) {
        throw std::runtime_error("Fail to listen socket\n");
    }
}

SocketPtr TcpSocket::Accept(struct sockaddr_in& clientAddr)
{
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = ::accept(GetFd(),
                            reinterpret_cast<struct sockaddr*>(&clientAddr),
                            &clientAddrLen);
    SetErrno(errno);
    if (clientFd == -1) {
        return nullptr;
    } else {
        return std::make_shared<TcpSocket>(clientFd);
    }
}

TcpSocket::~TcpSocket()
{
    ::close(GetFd());
}
