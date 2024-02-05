#pragma once

#include "DnsFlag.hpp"

class DnsHeader
{
public:
    DnsHeader() = default;
    void SetId(uint16_t dnsId);
    void SetFlag(const DnsFlag& flag);
    void SetQuestionCount(uint16_t dnsQuesCnt);
    void SetAnswerCount(uint16_t dnsAnsCnt);
    void SetAuthRecordCount(uint16_t dnsAuthRecCnt);
    void SetAdditionRecordCount(uint16_t dnsAddRecCnt);
    [[nodiscard]] uint16_t GetId() const;
    [[nodiscard]] DnsFlag GetFlag() const;
    [[nodiscard]] uint16_t GetQuestionCount() const;
    [[nodiscard]] uint16_t GetAnswerCount() const;
    [[nodiscard]] uint16_t GetAuthRecordCount() const;
    [[nodiscard]] uint16_t GetAdditionRecordCount() const;
    [[nodiscard]] std::string Stringify() const;
    void Parse(const std::string& udpMsg, size_t& pos);

private:
    uint16_t id_ = 0;
    DnsFlag flag_;
    uint16_t quesCnt_ = 0;
    uint16_t ansCnt_ = 0;
    uint16_t authRecCnt_ = 0;
    uint16_t addRecCnt_ = 0;
};

template<>
struct fmt::formatter<DnsHeader>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsHeader& header, FormatContext& ctx)
    {
        return fmt::format_to(
            ctx.out(),
            "ID: {:02X}, Flag: {}, Question Count: {:02X}, "
            "Answer Count: {:02X}, Auth Record Count: {:02X}, "
            "Addition Record Count: {:02X}",
            header.GetId(),
            header.GetFlag(),
            header.GetQuestionCount(),
            header.GetAnswerCount(),
            header.GetAuthRecordCount(),
            header.GetAdditionRecordCount());
    }
};
