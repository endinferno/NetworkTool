#include <thread>

#include "Connector.hpp"

#include "gtest/gtest.h"

bool isConnect = false;

void HandleNewConnection([[maybe_unused]] ChannelPtr chan)
{
    isConnect = true;
}

TEST(ConnectorTest, TcpConnector)
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    Connector connector(poller);
    connector.SetNewConnectionCallback(
        [](ChannelPtr&& chan) { HandleNewConnection(chan); });
    connector.Connect(IPAddress("223.5.5.5"), 80);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(isConnect, true);
}
