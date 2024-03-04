#include "Dns/DoHResolver.hpp"

void HandleDnsAnswer(const IPAddress& serverIp)
{
    DEBUG("IP {}\n", serverIp);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    DoHResolver resolver(poller);
    resolver.SetDnsMessageCallback(
        [](const IPAddress& serverIp) { HandleDnsAnswer(serverIp); });
    resolver.RequestIp("www.baidu.com");
    while (true) {}
}
