#pragma once

#include "Client/UdpClient.hpp"
#include "DnsMessage.hpp"

class DnsResolver
{
public:
    using DnsMessageCallback = std::function<void(IPAddress)>;

    explicit DnsResolver(EventPollerPtr& poller);
    void RequestIp(const std::string& domainName);
    void SetDnsMessageCallback(DnsMessageCallback callback);
    ~DnsResolver() = default;

private:
    bool HandleDnsMessage(const std::string& udpMsg);
    void SendDnsRequest(ChannelPtr chan);

    const std::string dnsServerIp_ = "223.5.5.5";
    const uint16_t dnsServerPort_ = 53;
    DnsMessageCallback callback_;
    UdpClient client_;
    DnsMessage request_;
};

using DnsResolverPtr = std::shared_ptr<DnsResolver>;
