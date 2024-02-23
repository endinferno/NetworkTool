#include "Server/UdpServer.hpp"
#include "Utils/Logger.hpp"

void OnMessageCallback([[maybe_unused]] ChannelPtr& chan,
                       const std::string& msg)
{
    DEBUG("{}\n", msg);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    UdpServer server(poller);
    server.SetOnMessageCallback([](ChannelPtr& chan, const std::string& msg) {
        OnMessageCallback(chan, msg);
    });
    IPAddress localIp(INADDR_ANY);
    uint16_t localPort = 8888;
    server.Run(localIp, localPort);

    while (true) {}

    return 0;
}
