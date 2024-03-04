#pragma once

#include "HttpsClient.hpp"

class DoHResolver
{
public:
    explicit DoHResolver(EventPollerPtr& poller);
    void RequestIp(const std::string& domainName);
    void SetDnsMessageCallback(DnsResolver::DnsMessageCallback&& callback);
    ~DoHResolver() = default;

private:
    void SendDnsRequest(ChannelPtr& chan);
    void HandleDnsMessage(const std::string& msg);

    const std::string aliDnsServerIp_ = "223.5.5.5";
    const uint16_t aliDnsServerPort_ = 443;
    const std::string aliDnsServerUrl_ = "/dns-query";
    const std::string aliDnsServerHost_ = "dns.alidns.com";
    const std::string contentType_ = "application/dns-message";
    DnsResolver::DnsMessageCallback callback_;
    HttpsClient client_;
    DnsMessage request_;
};
