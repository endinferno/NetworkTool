#include "Client/UdpClient.hpp"
#include "Utils/Logger.hpp"
#include <chrono>

void OnMessageCallback([[maybe_unused]] ChannelPtr& chan,
                       const std::string& msg)
{
    DEBUG("{}\n", msg);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    UdpClient client(poller);
    client.SetMessageCallback([](ChannelPtr& chan, const std::string& msg) {
        OnMessageCallback(chan, msg);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    });
    IPAddress localIp("127.0.0.1");
    uint16_t localPort = 8888;
    client.Connect(localIp, localPort);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    client.Write("Hello");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    client.Write("World");

    while (true) {}

    return 0;
}
