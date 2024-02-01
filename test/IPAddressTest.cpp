#include "IPAddress.hpp"

#include "gtest/gtest.h"

TEST(IPAddressTest, IPAddressStrToInt)
{
    std::string ipAddrStr = "192.168.1.2";
    IPAddress addr(ipAddrStr);
    EXPECT_EQ(addr.GetHostIpAddr(), 0xC0A80102);
    EXPECT_EQ(addr.GetNetIpAddr(), 0x0201A8C0);
    EXPECT_EQ(addr.Stringify(), "192.168.1.2");
}

TEST(IPAddressTest, IPAddressIntToStr)
{
    uint32_t ipAddrInt = 0xC0A80102;
    IPAddress addr(ipAddrInt);
    EXPECT_EQ(addr.GetHostIpAddr(), 0xC0A80102);
    EXPECT_EQ(addr.GetNetIpAddr(), 0x0201A8C0);
    EXPECT_EQ(addr.Stringify(), "192.168.1.2");
}
