#include <arpa/inet.h>

#include <array>

#include "IPAddress.hpp"

IPAddress::IPAddress(uint32_t hostIpAddr)
    : hostIp_(hostIpAddr)
{}

IPAddress::IPAddress(const std::string& ipAddr)
{
    uint32_t ipAddrInt;
    ::inet_pton(AF_INET, ipAddr.data(), &ipAddrInt);
    hostIp_ = ntohl(ipAddrInt);
}

uint32_t IPAddress::GetHostIpAddr() const
{
    return hostIp_;
}

uint32_t IPAddress::GetNetIpAddr() const
{
    return htonl(hostIp_);
}

std::string IPAddress::Stringify() const
{
    uint32_t netIpAddr = htonl(hostIp_);
    std::array<char, 20> ipAddr;
    ::inet_ntop(AF_INET, &netIpAddr, ipAddr.data(), 20);
    return { ipAddr.data() };
}
