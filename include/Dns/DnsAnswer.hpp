#pragma once

#include <any>
#include <stdexcept>

#include "DnsBaseType.hpp"
#include "Utils/IPAddress.hpp"
#include "Utils/Logger.hpp"

class DnsAnswer
{
public:
    DnsAnswer() = default;
    [[nodiscard]] std::string GetDomain() const;
    [[nodiscard]] DnsBaseType::DnsType GetType() const;
    [[nodiscard]] DnsBaseType::DnsClass GetClass() const;
    [[nodiscard]] uint32_t GetTtl() const;
    [[nodiscard]] uint16_t GetDataLen() const;
    [[nodiscard]] std::any GetData() const;
    void Parse(const std::string& udpMsg, size_t& pos);

private:
    bool IsDomainCompress(uint8_t domainCnt);
    std::string ParseDomain(const std::string& udpMsg, size_t& pos);
    IPAddress ParseAData(const std::string& udpMsg, size_t& pos);

    std::string domainName_;
    DnsBaseType::DnsType ansType_;
    DnsBaseType::DnsClass ansClass_;
    uint32_t ansTtl_;
    uint16_t ansDataLen_;
    std::any ansData_;
};

template<>
struct fmt::formatter<DnsAnswer>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsAnswer& answer, FormatContext& ctx)
    {
        auto type = answer.GetType();
        std::string formatData;
        switch (type) {
        case DnsBaseType::DnsType::A:
        {
            formatData =
                fmt::format("{}", std::any_cast<IPAddress>(answer.GetData()));
            break;
        }
        case DnsBaseType::DnsType::CNAME:
        {
            formatData =
                fmt::format("{}", std::any_cast<std::string>(answer.GetData()));
            break;
        }
        default:
            throw std::runtime_error(
                fmt::format("Cannot print such dns type data {}",
                            static_cast<uint16_t>(type)));
        }
        return fmt::format_to(
            ctx.out(),
            "Domain: {} Type: {} Class: {} TTL: {} DataLen: {:02X} Data: {}",
            answer.GetDomain(),
            type,
            answer.GetClass(),
            answer.GetTtl(),
            answer.GetDataLen(),
            formatData);
    }
};
