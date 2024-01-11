#include "Signal.hpp"
#include "SinaStockClient.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    EpollerPtr epoller = std::make_shared<Epoller>();
    epoller->Run();

    SinaStockClient client(epoller);
    client.Connect();

    while (!signal.IsSignalTrigger()) {
        client.GetStock("sz002603");
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
