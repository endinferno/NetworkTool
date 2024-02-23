#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "Socket/UdpSocket.hpp"

UdpSocket::UdpSocket()
    : Socket(::socket(AF_INET, SOCK_DGRAM, 0))
{}

void UdpSocket::Bind(const IPAddress& localIp, const uint16_t& localPort)
{
    serverAddr_.first = localIp;
    serverAddr_.second = localPort;

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

void UdpSocket::Listen()
{
    // Do nothing
}

SocketPtr UdpSocket::Accept(struct sockaddr_in& clientAddr)
{
    std::string readBuf(1024, '0');
    ssize_t readBytes = Recvfrom(readBuf, clientAddr);

    if (readBytes < 0) {
        SetErrno(errno);
        return nullptr;
    }
    auto sock = std::make_shared<UdpSocket>();
    sock->SetReuseAddr();
    sock->SetReusePort();
    sock->SetNonBlock();
    sock->Bind(serverAddr_.first, serverAddr_.second);
    sock->Connect(clientAddr);

    return sock;
}

UdpSocket::~UdpSocket()
{
    ::close(GetFd());
}
