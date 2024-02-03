#pragma once

#include <string>

class IPAddress
{
public:
    explicit IPAddress(uint32_t hostIpAddr);
    explicit IPAddress(const std::string& hostIpAddr);
    [[nodiscard]] uint32_t GetHostIpAddr() const;
    [[nodiscard]] uint32_t GetNetIpAddr() const;
    [[nodiscard]] std::string Stringify() const;
    ~IPAddress() = default;

private:
    uint32_t hostIp_;
};
