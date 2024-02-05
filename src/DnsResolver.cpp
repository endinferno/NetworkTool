#include "DnsResolver.hpp"
#include "Utils/Logger.hpp"

DnsResolver::DnsResolver(EventPollerPtr& poller)
    : client_(poller)
{
    client_.SetOnMessageCallback(
        [this](const std::string& udpMsg) { HandleDnsMessage(udpMsg); });
    client_.SetConnectDoneCallback(
        [this](ChannelPtr&& chan) { SendDnsRequest(chan); });
}

void DnsResolver::RequestIp(const std::string& domainName)
{
    DnsFlag dnsFlag;
    dnsFlag.SetRD(true);
    request_.SetHeaderFlag(dnsFlag);
    request_.AddDomain(domainName);
    client_.Connect(IPAddress(dnsServerIp_), dnsServerPort_);
}

void DnsResolver::SetDnsMessageCallback(DnsMessageCallback callback)
{
    callback_ = std::move(callback);
}

void DnsResolver::HandleDnsMessage(const std::string& udpMsg)
{
    DEBUG("Handle dns message\n");
    DnsMessage dnsMsg;
    dnsMsg.Parse(udpMsg);
    auto answers = dnsMsg.GetAnswer();
    IPAddress ipAddr;
    for (const auto& answer : answers) {
        if (answer.GetType() == DnsBaseType::DnsType::A) {
            ipAddr = std::any_cast<IPAddress>(answer.GetData());
            break;
        }
    }
    if (callback_ != nullptr) {
        callback_(ipAddr);
    }
}

void DnsResolver::SendDnsRequest([[maybe_unused]] ChannelPtr chan)
{
    DEBUG("Send dns request\n");
    auto reqMsg = request_.Stringify();
    client_.Write(reqMsg);
}
