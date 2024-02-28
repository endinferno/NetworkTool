#include "EventFdClient.hpp"
#include "Utils/Logger.hpp"

void ReadCallback(eventfd_t& event)
{
    DEBUG("Read eventFd {}\n", event);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    EventFdClient client(poller);
    client.SetReadCallback([](eventfd_t& event) { ReadCallback(event); });
    size_t idx = client.Create();
    int cnt = 0;
    while (cnt < 10) {
        int ret = client.Write(idx, 123);
        if (ret == -1) {
            continue;
        } else {
            cnt++;
        }
    }
    while (true) {}
    return 0;
}
