#include "Dns/DnsAnswer.hpp"

std::string DnsAnswer::GetDomain() const
{
    return domainName_;
}

DnsBaseType::DnsType DnsAnswer::GetType() const
{
    return ansType_;
}

DnsBaseType::DnsClass DnsAnswer::GetClass() const
{
    return ansClass_;
}

uint32_t DnsAnswer::GetTtl() const
{
    return ansTtl_;
}

uint16_t DnsAnswer::GetDataLen() const
{
    return ansDataLen_;
}

std::any DnsAnswer::GetData() const
{
    return ansData_;
}

void DnsAnswer::Parse(const std::string& udpMsg, size_t& pos)
{
    const char* udpMsgPtr = udpMsg.data();
    domainName_ = ParseDomain(udpMsg, pos);

    uint16_t ansTypeInt;
    ::memcpy(&ansTypeInt, udpMsgPtr + pos, sizeof(uint16_t));
    ansType_ = static_cast<DnsBaseType::DnsType>(ntohs(ansTypeInt));
    pos += sizeof(ansTypeInt);

    uint16_t ansClassInt;
    ::memcpy(&ansClassInt, udpMsgPtr + pos, sizeof(uint16_t));
    ansClass_ = static_cast<DnsBaseType::DnsClass>(ntohs(ansClassInt));
    pos += sizeof(ansClassInt);

    ::memcpy(&ansTtl_, udpMsgPtr + pos, sizeof(uint32_t));
    ansTtl_ = ntohl(ansTtl_);
    pos += sizeof(ansTtl_);

    ::memcpy(&ansDataLen_, udpMsgPtr + pos, sizeof(uint16_t));
    ansDataLen_ = ntohs(ansDataLen_);
    pos += sizeof(ansDataLen_);

    switch (ansType_) {
    case DnsBaseType::DnsType::A:
    {
        ansData_ = ParseAData(udpMsg, pos);
        break;
    }
    case DnsBaseType::DnsType::CNAME:
    {
        ansData_ = ParseDomain(udpMsg, pos);
        break;
    }
    default:
        throw std::runtime_error(
            fmt::format("Cannot parse such dns type data {}",
                        static_cast<uint16_t>(ansType_)));
    }
}

bool DnsAnswer::IsDomainCompress(uint8_t domainCnt)
{
    return (domainCnt & 0xC0) == 0xC0;
}

// NOLINTNEXTLINE(misc-no-recursion)
std::string DnsAnswer::ParseDomain(const std::string& udpMsg, size_t& pos)
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

IPAddress DnsAnswer::ParseAData(const std::string& udpMsg, size_t& pos)
{
    uint32_t ipAddrInt =
        ntohl(*reinterpret_cast<const uint32_t*>(udpMsg.data() + pos));
    pos += sizeof(uint32_t);
    return IPAddress(ipAddrInt);
}
