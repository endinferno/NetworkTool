#include <arpa/inet.h>

#include <array>

#include "IPAddress.hpp"

IPAddress::IPAddress(uint32_t ipAddr)
    : ip_(ipAddr)
{}

IPAddress::IPAddress(const std::string& ipAddr)
{
    uint32_t ipAddrInt;
    ::inet_pton(AF_INET, ipAddr.data(), &ipAddrInt);
    ip_ = ntohl(ipAddrInt);
}

uint32_t IPAddress::GetHostIpAddr() const
{
    return ip_;
}

uint32_t IPAddress::GetNetIpAddr() const
{
    return htonl(ip_);
}

std::string IPAddress::Stringify() const
{
    uint32_t netIpAddr = ntohl(ip_);
    std::array<char, 20> ipAddr;
    ::inet_ntop(AF_INET, &netIpAddr, ipAddr.data(), 20);
    return { ipAddr.data() };
}
