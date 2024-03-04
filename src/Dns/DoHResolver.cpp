#include "Dns/DoHResolver.hpp"
#include "Utils/Base64.hpp"

DoHResolver::DoHResolver(EventPollerPtr& poller)
    : client_(poller)
{
    client_.SetMessageDecodeCallback(
        [this](const std::string& msg) { HandleDnsMessage(msg); });
    client_.SetWriteCompleteCallback(
        [this](ChannelPtr& chan) { SendDnsRequest(chan); });
}

void DoHResolver::RequestIp(const std::string& domainName)
{
    DnsFlag dnsFlag;
    dnsFlag.SetRD(true);
    request_.SetHeaderFlag(dnsFlag);
    request_.AddDomain(domainName);
    client_.Connect(IPAddress(aliDnsServerIp_), aliDnsServerPort_);
}

void DoHResolver::SetDnsMessageCallback(
    DnsResolver::DnsMessageCallback&& callback)
{
    callback_ = std::move(callback);
}

void DoHResolver::SendDnsRequest([[maybe_unused]] ChannelPtr& chan)
{
    DEBUG("Send dns request\n");
    auto reqMsg = request_.Stringify();
    HttpRequest httpReq;
    httpReq.SetReqType(HttpRequest::ReqType::GET);
    httpReq.SetUrl(aliDnsServerUrl_);
    httpReq.AddParam("dns", Base64::Encode(reqMsg, true));
    httpReq.AddHeader("Host", aliDnsServerHost_);
    httpReq.AddHeader("Content-Type", contentType_);
    client_.Request(httpReq);
}

void DoHResolver::HandleDnsMessage(const std::string& msg)
{
    DnsMessage dnsMsg;
    dnsMsg.Parse(msg);
    auto answers = dnsMsg.GetAnswer();
    for (const auto& answer : answers) {
        if (answer.GetType() != DnsBaseType::DnsType::A) {
            continue;
        }
        auto ipAddr = std::any_cast<IPAddress>(answer.GetData());
        if (callback_ != nullptr) {
            callback_(ipAddr);
        }
        break;
    }
}
