#include "Signal.hpp"
#include "SinaStockClient.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    Signal signal;

    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    SinaStockClient client(poller);
    client.Connect();

    while (!Signal::IsSignalTrigger()) {
        client.GetStock("sz002603");
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
