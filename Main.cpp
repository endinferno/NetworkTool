#include "Epoller.hpp"
#include "HttpRequest.hpp"
#include "Logger.hpp"
#include "Signal.hpp"
#include "TcpClient.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    std::shared_ptr<Epoller> epoller = std::make_shared<Epoller>();
    epoller->Run();

    std::shared_ptr<TcpClient> client = std::make_shared<TcpClient>(epoller);
    client->Connect("hq.sinajs.cn", 80);

    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl("/list=sz002603");
    httpReq.AddHeader("Host", "hq.sinajs.cn");
    httpReq.AddHeader("Referer", "http://finance.sina.com.cn");
    auto writeBuf = httpReq.GetBuffer();

    while (!signal.IsSignalTrigger()) {
        ssize_t writeBytes = client->Write(writeBuf);
        DEBUG("{}\n", writeBytes);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}
