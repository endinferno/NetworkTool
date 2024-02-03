#include <sys/socket.h>
#include <unistd.h>

#include "Socket/UdpSocket.hpp"

UdpSocket::UdpSocket()
    : Socket(::socket(AF_INET, SOCK_DGRAM, 0))
{}

UdpSocket::~UdpSocket()
{
    ::close(GetFd());
}
