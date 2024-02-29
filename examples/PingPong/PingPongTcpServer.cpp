#include "Server/TcpServer.hpp"
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

    TcpServer server(poller);
    server.SetMessageCallback([](ChannelPtr& chan, const std::string& msg) {
        OnMessageCallback(chan, msg);
    });
    IPAddress localIp("127.0.0.1");
    uint16_t localPort = 8888;
    server.Run(localIp, localPort);

    while (true) {}

    return 0;
}
