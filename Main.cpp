#include "Epoller.hpp"
#include "Logger.hpp"
#include "Signal.hpp"
#include "TcpClient.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    std::shared_ptr<Epoller> epoller = std::make_shared<Epoller>();
    epoller->Run();

    std::shared_ptr<TcpClient> client = std::make_shared<TcpClient>(epoller);
    client->Connect();

    std::string writeMsg;
    writeMsg += "GET /list=sz002603 HTTP/1.1\r\n";
    writeMsg += "Host: hq.sinajs.cn\r\n";
    writeMsg += "Referer: http://finance.sina.com.cn\r\n";
    writeMsg += "\r\n";

    std::vector<char> writeBuf(writeMsg.begin(), writeMsg.end());

    while (!signal.IsSignalTrigger()) {
        ssize_t writeBytes = client->Write(writeBuf);
        DEBUG("{}\n", writeBytes);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}
