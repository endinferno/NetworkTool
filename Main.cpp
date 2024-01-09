#include "HttpClient.hpp"
#include "Signal.hpp"

int main(int argc, char* argv[])
{
    Signal signal;

    EpollerPtr epoller = std::make_shared<Epoller>();
    epoller->Run();

    HttpClient client(epoller);
    client.Connect("hq.sinajs.cn");

    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl("/list=sz002603");
    httpReq.AddHeader("Host", "hq.sinajs.cn");
    httpReq.AddHeader("Referer", "http://finance.sina.com.cn");

    while (!signal.IsSignalTrigger()) {
        client.Request(httpReq);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
