#pragma once

#include "DnsBaseType.hpp"
#include "Utils/Logger.hpp"

class DnsQuestion
{
public:
    DnsQuestion() = default;
    void SetDomain(const std::string& domain);
    void SetType(const DnsBaseType::DnsType& dnsType);
    void SetClass(const DnsBaseType::DnsClass& dnsClass);
    [[nodiscard]] std::string GetDomain() const;
    [[nodiscard]] DnsBaseType::DnsType GetType() const;
    [[nodiscard]] DnsBaseType::DnsClass GetClass() const;
    [[nodiscard]] std::string Stringify() const;
    void Parse(const std::string& udpMsg, size_t& pos);

private:
    bool IsDomainCompress(uint8_t domainCnt);
    std::string ParseDomain(const std::string& udpMsg, size_t& pos);

    std::string domainName_;
    DnsBaseType::DnsType quesType_;
    DnsBaseType::DnsClass quesClass_;
};

template<>
struct fmt::formatter<DnsQuestion>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsQuestion& question, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(),
                              "Domain: {} Type: {} Class: {}",
                              question.GetDomain(),
                              question.GetType(),
                              question.GetClass());
    }
};
