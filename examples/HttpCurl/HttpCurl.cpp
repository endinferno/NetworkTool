#include "HttpClient.hpp"

void OnMessage(const std::string& msg)
{
    DEBUG("{}\n", msg);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    HttpClient client(poller);
    std::string serverName = "www.baidu.com";
    uint16_t serverPort = 80;
    client.Connect(serverName, serverPort);
    client.SetMessageDecodeCallback(
        [](const std::string& msg) { OnMessage(msg); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl("/");
    httpReq.AddHeader("Host", "www.baidu.com");
    httpReq.AddHeader("User-Agent", "curl/7.68.0");
    httpReq.AddHeader("Accept", "*/*");
    client.Request(httpReq);

    while (true) {}
    return 0;
}
