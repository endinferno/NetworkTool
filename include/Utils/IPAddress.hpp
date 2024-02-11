#pragma once

#include <arpa/inet.h>

#include <array>
#include <string>

#include "fmt/format.h"

class IPAddress
{
public:
    explicit IPAddress()
        : hostIp_(0)
    {}
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

// NOLINTBEGIN(readability-identifier-naming)
template<>
struct fmt::formatter<IPAddress>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const IPAddress& ipAddr, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", ipAddr.Stringify());
    }
};
// NOLINTEND(readability-identifier-naming)
