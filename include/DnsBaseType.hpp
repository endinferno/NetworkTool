#pragma once

#include <string>

#include <arpa/inet.h>

#include "fmt/format.h"

class DnsBaseType
{
public:
    enum DnsType
    {
        A = 1,
        NS = 2,
        CNAME = 5,
        PTR = 12,
        HINFO = 13,
        MX = 15,
    };
    enum DnsClass
    {
        Internet = 1,
    };
    enum QRFlag
    {
        Query,
        Response,
    };
    enum OpCodeFlag
    {
        Standard,
        Reverse,
        Status,
    };
    enum RespCodeFlag
    {
        Ok,
        NameErr = 3,
    };
    [[nodiscard]] static std::string Stringify(const DnsType& dnsType);
    [[nodiscard]] static std::string Stringify(const DnsClass& dnsClass);
};

template<>
struct fmt::formatter<DnsBaseType::DnsType>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsBaseType::DnsType& dnsType, FormatContext& ctx)
    {
        std::string formatStr;
        switch (dnsType) {
        case DnsBaseType::DnsType::A: formatStr = "A"; break;
        case DnsBaseType::DnsType::NS: formatStr = "NS"; break;
        case DnsBaseType::DnsType::CNAME: formatStr = "CNAME"; break;
        case DnsBaseType::DnsType::PTR: formatStr = "PTR"; break;
        case DnsBaseType::DnsType::HINFO: formatStr = "HINFO"; break;
        case DnsBaseType::DnsType::MX: formatStr = "MX"; break;
        default:
            throw std::runtime_error(fmt::format(
                "No such dns type {:04X}\n", static_cast<uint16_t>(dnsType)));
        }
        return fmt::format_to(ctx.out(), "{}", formatStr);
    }
};

template<>
struct fmt::formatter<DnsBaseType::DnsClass>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsBaseType::DnsClass& dnsClass, FormatContext& ctx)
    {
        std::string formatStr;
        switch (dnsClass) {
        case DnsBaseType::DnsClass::Internet: formatStr = "Internet"; break;
        default:
            throw std::runtime_error(fmt::format(
                "No such dns class {:04X}\n", static_cast<uint16_t>(dnsClass)));
        }
        return fmt::format_to(ctx.out(), "{}", formatStr);
    }
};

template<>
struct fmt::formatter<DnsBaseType::QRFlag>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsBaseType::QRFlag& flag, FormatContext& ctx)
    {
        switch (flag) {
        case DnsBaseType::QRFlag::Query:
            return fmt::format_to(ctx.out(), "{}", "Query");
        case DnsBaseType::QRFlag::Response:
            return fmt::format_to(ctx.out(), "{}", "Response");
        default:
            throw std::runtime_error(
                fmt::format("No such QR flag {}", static_cast<uint8_t>(flag)));
        }
    }
};

template<>
struct fmt::formatter<DnsBaseType::OpCodeFlag>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsBaseType::OpCodeFlag& flag, FormatContext& ctx)
    {
        switch (flag) {
        case DnsBaseType::OpCodeFlag::Standard:
            return fmt::format_to(ctx.out(), "{}", "Standard");
        case DnsBaseType::OpCodeFlag::Reverse:
            return fmt::format_to(ctx.out(), "{}", "Reverse");
        case DnsBaseType::OpCodeFlag::Status:
            return fmt::format_to(ctx.out(), "{}", "Status");
        default:
            throw std::runtime_error(fmt::format("No such opcode flag {}",
                                                 static_cast<uint8_t>(flag)));
        }
    }
};

template<>
struct fmt::formatter<DnsBaseType::RespCodeFlag>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsBaseType::RespCodeFlag& flag, FormatContext& ctx)
    {
        switch (flag) {
        case DnsBaseType::RespCodeFlag::Ok:
            return fmt::format_to(ctx.out(), "{}", "OK");
        case DnsBaseType::RespCodeFlag::NameErr:
            return fmt::format_to(ctx.out(), "{}", "Name Error");
        default:
            throw std::runtime_error(fmt::format("No such response code {}",
                                                 static_cast<uint8_t>(flag)));
        }
    }
};
