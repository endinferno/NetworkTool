#include "DnsFlag.hpp"

#include "gtest/gtest.h"

TEST(DnsResolver, DnsFlag)
{
    DnsFlag dnsFlag;
    auto dnsStr = dnsFlag.Stringify();
    EXPECT_EQ(dnsStr.size(), 2);
    EXPECT_EQ(dnsStr[0], static_cast<char>(0x00));
    EXPECT_EQ(dnsStr[1], static_cast<char>(0x00));

    dnsFlag.SetQR(DnsBaseType::QRFlag::Response);
    dnsStr = dnsFlag.Stringify();
    EXPECT_EQ(dnsStr.size(), 2);
    EXPECT_EQ(dnsStr[0], static_cast<char>(0x80));
    EXPECT_EQ(dnsStr[1], static_cast<char>(0x00));

    dnsFlag.SetOpCode(DnsBaseType::OpCodeFlag::Status);
    dnsStr = dnsFlag.Stringify();
    EXPECT_EQ(dnsStr.size(), 2);
    EXPECT_EQ(dnsStr[0], static_cast<char>(0x90));
    EXPECT_EQ(dnsStr[1], static_cast<char>(0x00));
}
