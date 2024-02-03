#include <thread>

#include "SslConnector.hpp"
#include "TcpConnector.hpp"
#include "UdpConnector.hpp"

#include "gtest/gtest.h"

bool isConnect = false;

void HandleNewConnection([[maybe_unused]] ChannelPtr chan)
{
    isConnect = true;
}

TEST(ConnectorTest, TcpConnector)
{
    const IPAddress serverIp("223.5.5.5");
    const uint16_t serverPort = 80;

    isConnect = false;
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    TcpConnector connector(poller);
    connector.SetNewConnectionCallback(
        [](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector.Connect(serverIp, serverPort);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(isConnect, true);
}

TEST(ConnectorTest, UdpConnector)
{
    const IPAddress serverIp("223.5.5.5");
    const uint16_t serverPort = 53;

    isConnect = false;
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    UdpConnector connector(poller);
    connector.SetNewConnectionCallback(
        [](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector.Connect(serverIp, serverPort);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(isConnect, true);
}

TEST(ConnectorTest, SslConnector)
{
    const IPAddress serverIp("223.5.5.5");
    const uint16_t serverPort = 443;

    isConnect = false;
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    SslConnector connector(poller);
    connector.SetNewConnectionCallback(
        [](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector.Connect(serverIp, serverPort);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(isConnect, true);
}
