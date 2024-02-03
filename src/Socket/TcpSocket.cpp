#include <sys/socket.h>
#include <unistd.h>

#include "Socket/TcpSocket.hpp"

TcpSocket::TcpSocket()
    : Socket(::socket(AF_INET, SOCK_STREAM, 0))
{}

TcpSocket::~TcpSocket()
{
    ::close(GetFd());
}
