#pragma once

#include <cstdint>

#include <string>

class IPAddress
{
public:
    explicit IPAddress(uint32_t ipAddr);
    explicit IPAddress(const std::string& ipAddr);
    [[nodiscard]] uint32_t GetHostIpAddr() const;
    [[nodiscard]] uint32_t GetNetIpAddr() const;
    [[nodiscard]] std::string Stringify() const;

private:
    uint32_t ip_;
};
