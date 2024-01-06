#include "Logger.hpp"
#include "Signal.hpp"
#include "TcpClient.hpp"
#include "TcpConnection.hpp"
#include "TcpConnector.hpp"
#include "TcpSocket.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    std::shared_ptr<TcpClient> client = std::make_shared<TcpClient>();
    client->Run();
    DEBUG("After Run\n");

    std::shared_ptr<TcpSocket> sock = std::make_shared<TcpSocket>();

    TcpConnector connector(client);
    auto conn = connector.Connect(sock);

    while (!signal.IsSignalTrigger()) {
        ;
    }

    return 0;
}
