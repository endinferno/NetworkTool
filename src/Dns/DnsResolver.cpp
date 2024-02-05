#include "Dns/DnsResolver.hpp"
#include "Utils/Logger.hpp"

DnsResolver::DnsResolver(EventPollerPtr& poller)
    : client_(poller)
{
    client_.SetOnMessageCallback(
        [this](ChannelPtr&& chan, const std::string& udpMsg) {
            HandleDnsMessage(chan, udpMsg);
        });
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

void DnsResolver::HandleDnsMessage(ChannelPtr chan, const std::string& udpMsg)
{
    DEBUG("Handle dns message\n");
    DnsMessage dnsMsg;
    dnsMsg.Parse(udpMsg);
    auto answers = dnsMsg.GetAnswer();
    for (const auto& answer : answers) {
        if (answer.GetType() != DnsBaseType::DnsType::A) {
            continue;
        }
        auto ipAddr = std::any_cast<IPAddress>(answer.GetData());
        if (callback_ != nullptr) {
            callback_(ipAddr);
        }
        client_.DelEvent(chan);
        break;
    }
}

void DnsResolver::SendDnsRequest([[maybe_unused]] ChannelPtr chan)
{
    DEBUG("Send dns request\n");
    auto reqMsg = request_.Stringify();
    client_.Write(reqMsg);
}
