#include "Dns/DnsQuestion.hpp"

void DnsQuestion::SetDomain(const std::string& domain)
{
    domainName_ = domain;
}

void DnsQuestion::SetType(const DnsBaseType::DnsType& dnsType)
{
    quesType_ = dnsType;
}

void DnsQuestion::SetClass(const DnsBaseType::DnsClass& dnsClass)
{
    quesClass_ = dnsClass;
}

std::string DnsQuestion::GetDomain() const
{
    return domainName_;
}

DnsBaseType::DnsType DnsQuestion::GetType() const
{
    return quesType_;
}

DnsBaseType::DnsClass DnsQuestion::GetClass() const
{
    return quesClass_;
}

std::string DnsQuestion::Stringify() const
{
    std::string content;
    uint8_t cnt = 0;
    std::string domain;
    for (const auto& domainCh : domainName_) {
        if (domainCh == '.') {
            content.push_back(static_cast<char>(cnt));
            content.append(domain);
            domain.clear();
            cnt = 0;
        } else {
            domain.push_back(domainCh);
            cnt++;
        }
    }
    content.push_back(static_cast<char>(cnt));
    content.append(domain);
    content.push_back(static_cast<char>(0x00));
    content.append(DnsBaseType::Stringify(quesType_));
    content.append(DnsBaseType::Stringify(quesClass_));
    return content;
}

void DnsQuestion::Parse(const std::string& udpMsg, size_t& pos)
{
    const char* udpMsgPtr = udpMsg.data();
    domainName_ = ParseDomain(udpMsg, pos);

    uint16_t quesTypeInt;
    ::memcpy(&quesTypeInt, udpMsgPtr + pos, sizeof(uint16_t));
    quesType_ = static_cast<DnsBaseType::DnsType>(ntohs(quesTypeInt));
    pos += sizeof(uint16_t);

    uint16_t quesClassInt;
    ::memcpy(&quesClassInt, udpMsgPtr + pos, sizeof(uint16_t));
    quesClass_ = static_cast<DnsBaseType::DnsClass>(ntohs(quesClassInt));
    pos += sizeof(uint16_t);
}

bool DnsQuestion::IsDomainCompress(uint8_t domainCnt)
{
    return (domainCnt & 0xC0) == 0xC0;
}

std::string DnsQuestion::ParseDomain(const std::string& udpMsg, size_t& pos)
{
    std::string domainName;
    const char* udpMsgPtr = udpMsg.data();
    uint8_t domainCnt = *reinterpret_cast<const uint8_t*>(udpMsgPtr + pos);
    pos++;
    while (domainCnt != 0) {
        if (IsDomainCompress(domainCnt)) {
            uint8_t lowerCnt =
                *reinterpret_cast<const uint8_t*>(udpMsgPtr + pos);
            pos++;
            auto newPos = static_cast<size_t>(
                (static_cast<uint16_t>(domainCnt) ^ 0xC0) << 8 |
                static_cast<uint16_t>(lowerCnt));
            domainName.append(ParseDomain(udpMsg, newPos));
            break;
        }
        domainName.append(std::string(udpMsgPtr + pos, domainCnt));
        pos += domainCnt;
        domainCnt = *reinterpret_cast<const uint8_t*>(udpMsgPtr + pos);
        pos++;
        if (domainCnt != 0) {
            domainName.push_back('.');
        }
    }
    return domainName;
}
