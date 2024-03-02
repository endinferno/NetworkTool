#include "HttpsClient.hpp"

void OnMessage(const std::string& msg)
{
    DEBUG("{}\n", msg);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    EventPollerPtr poller = std::make_shared<EventPoller>();
    poller->Run();

    HttpsClient client(poller);
    std::string serverName = "www.baidu.com";
    uint16_t serverPort = 443;
    client.Connect(serverName, serverPort);
    client.SetMessageDecodeCallback(
        [](const std::string& msg) { OnMessage(msg); });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl("/");
    httpReq.AddHeader("Host", "www.baidu.com");
    client.Request(httpReq);

    while (true) {}
    return 0;
}
