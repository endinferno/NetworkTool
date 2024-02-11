#pragma once

#include "DnsBaseType.hpp"

class DnsFlag
{
public:
    DnsFlag() = default;
    explicit DnsFlag(uint16_t flag);
    void SetQR(DnsBaseType::QRFlag qrFlag);
    void SetOpCode(DnsBaseType::OpCodeFlag opcodeFlag);
    void SetAA(bool aaFlag);
    void SetTC(bool tcFlag);
    void SetRD(bool rdFlag);
    void SetRA(bool raFlag);
    void SetRespCode(DnsBaseType::RespCodeFlag respCodeFlag);

    [[nodiscard]] DnsBaseType::QRFlag GetQR() const;
    [[nodiscard]] DnsBaseType::OpCodeFlag GetOpCode() const;
    [[nodiscard]] bool GetAA() const;
    [[nodiscard]] bool GetTC() const;
    [[nodiscard]] bool GetRD() const;
    [[nodiscard]] bool GetRA() const;
    [[nodiscard]] DnsBaseType::RespCodeFlag GetRespCode() const;
    [[nodiscard]] std::string Stringify() const;
    void Parse(const std::string& udpMsg, size_t& pos);
    void Decode(uint16_t flag);
    [[nodiscard]] uint16_t Encode() const;

private:
    // QR | OpCode | AA | TC | RD | RA | Reversed | RespCode
    // 0  |  0000  | 0  | 0  | 0  | 0  |   000    |  0000
    DnsBaseType::QRFlag QRFlag_ = DnsBaseType::QRFlag::Query;
    DnsBaseType::OpCodeFlag opCode_ = DnsBaseType::OpCodeFlag::Standard;
    bool AA_ = false;
    bool TC_ = false;
    bool RD_ = false;
    bool RA_ = false;
    DnsBaseType::RespCodeFlag respCode_ = DnsBaseType::RespCodeFlag::Ok;
};

// NOLINTBEGIN(readability-identifier-naming)
template<>
struct fmt::formatter<DnsFlag>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsFlag& flag, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(),
                              "{}, {}, {}{}{}{}{}, {:04X}",
                              flag.GetQR(),
                              flag.GetOpCode(),
                              flag.GetAA() ? "Authoritative Answer, " : "",
                              flag.GetTC() ? "Truncated, " : "",
                              flag.GetRD() ? "Recursion Desired, " : "",
                              flag.GetRA() ? "Recursion Available, " : "",
                              flag.GetRespCode(),
                              flag.Encode());
    }
};
// NOLINTEND(readability-identifier-naming)
