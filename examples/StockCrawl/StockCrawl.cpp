#include "SinaStockClient.hpp"
#include "Utils/Signal.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Signal sig;

    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    SinaStockClient client(poller);
    client.Connect();

    DEBUG("13\n");
    while (!Signal::IsSignalTrigger()) {
        DEBUG("14\n");
        client.GetStock("sz002603");
        DEBUG("15\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
