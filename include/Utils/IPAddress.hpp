#pragma once

#include <arpa/inet.h>

#include <array>
#include <string>

class IPAddress
{
public:
    explicit IPAddress(uint32_t hostIpAddr)
        : hostIp_(hostIpAddr)
    {}
    explicit IPAddress(const std::string& hostIpAddr)
    {
        uint32_t ipAddrInt = 0;
        ::inet_pton(AF_INET, hostIpAddr.data(), &ipAddrInt);
        hostIp_ = ntohl(ipAddrInt);
    }
    [[nodiscard]] uint32_t GetHostIpAddr() const
    {
        return hostIp_;
    }
    [[nodiscard]] uint32_t GetNetIpAddr() const
    {
        return htonl(hostIp_);
    }
    [[nodiscard]] std::string Stringify() const
    {
        uint32_t netIpAddr = htonl(hostIp_);
        std::array<char, 20> ipAddr;
        ::inet_ntop(AF_INET, &netIpAddr, ipAddr.data(), 20);
        return { ipAddr.data() };
    }
    ~IPAddress() = default;

private:
    uint32_t hostIp_;
};
