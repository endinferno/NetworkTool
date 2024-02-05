#include <cstdint>

#include "DnsBaseType.hpp"

std::string DnsBaseType::Stringify(const DnsType& dnsType)
{
    std::string quesTypeStr(sizeof(uint16_t), '0');
    auto quesType = htons(static_cast<uint16_t>(dnsType));
    ::memcpy(quesTypeStr.data(), &quesType, sizeof(uint16_t));
    return quesTypeStr;
}

std::string DnsBaseType::Stringify(const DnsClass& dnsClass)
{
    auto quesClass = htons(static_cast<uint16_t>(dnsClass));
    std::string quesClassStr(sizeof(uint16_t), '0');
    ::memcpy(quesClassStr.data(), &quesClass, sizeof(uint16_t));
    return quesClassStr;
}
