#include "Epoller.hpp"
#include "Logger.hpp"
#include "Signal.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"
#include "TcpSocket.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    std::shared_ptr<Epoller> epoller = std::make_shared<Epoller>();
    epoller->Run();
    DEBUG("After Run\n");

    std::shared_ptr<TcpSocket> sock = std::make_shared<TcpSocket>();

    TcpConnector connector(epoller);
    auto conn = connector.Connect(sock);

    while (!signal.IsSignalTrigger()) {
        ;
    }

    return 0;
}
