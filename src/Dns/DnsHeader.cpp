#include "Dns/DnsHeader.hpp"

void DnsHeader::SetId(uint16_t dnsId)
{
    id_ = dnsId;
}

void DnsHeader::SetFlag(const DnsFlag& flag)
{
    flag_ = flag;
}

void DnsHeader::SetQuestionCount(uint16_t dnsQuesCnt)
{
    quesCnt_ = dnsQuesCnt;
}

void DnsHeader::SetAnswerCount(uint16_t dnsAnsCnt)
{
    ansCnt_ = dnsAnsCnt;
}

void DnsHeader::SetAuthRecordCount(uint16_t dnsAuthRecCnt)
{
    authRecCnt_ = dnsAuthRecCnt;
}

void DnsHeader::SetAdditionRecordCount(uint16_t dnsAddRecCnt)
{
    addRecCnt_ = dnsAddRecCnt;
}

uint16_t DnsHeader::GetId() const
{
    return id_;
}

DnsFlag DnsHeader::GetFlag() const
{
    return flag_;
}

uint16_t DnsHeader::GetQuestionCount() const
{
    return quesCnt_;
}

uint16_t DnsHeader::GetAnswerCount() const
{
    return ansCnt_;
}

uint16_t DnsHeader::GetAuthRecordCount() const
{
    return authRecCnt_;
}

uint16_t DnsHeader::GetAdditionRecordCount() const
{
    return addRecCnt_;
}

std::string DnsHeader::Stringify() const
{
    std::string content(12, '0');
    char* contentPtr = content.data();
    uint16_t dnsId = htons(id_);
    uint16_t quesCnt = htons(quesCnt_);
    uint16_t ansCnt = htons(ansCnt_);
    uint16_t authRecCnt = htons(authRecCnt_);
    uint16_t addRecCnt = htons(addRecCnt_);
    ::memcpy(contentPtr, &dnsId, sizeof(uint16_t));
    ::memcpy(contentPtr + 2, flag_.Stringify().data(), sizeof(uint16_t));
    ::memcpy(contentPtr + 4, &quesCnt, sizeof(uint16_t));
    ::memcpy(contentPtr + 6, &ansCnt, sizeof(uint16_t));
    ::memcpy(contentPtr + 8, &authRecCnt, sizeof(uint16_t));
    ::memcpy(contentPtr + 10, &addRecCnt, sizeof(uint16_t));
    return content;
}

void DnsHeader::Parse(const std::string& udpMsg, size_t& pos)
{
    const char* udpMsgPtr = udpMsg.data();
    ::memcpy(&id_, udpMsgPtr + pos, sizeof(uint16_t));
    pos += 2;
    flag_.Parse(udpMsg, pos);
    ::memcpy(&quesCnt_, udpMsgPtr + pos, sizeof(uint16_t));
    pos += 2;
    ::memcpy(&ansCnt_, udpMsgPtr + pos, sizeof(uint16_t));
    pos += 2;
    ::memcpy(&authRecCnt_, udpMsgPtr + pos, sizeof(uint16_t));
    pos += 2;
    ::memcpy(&addRecCnt_, udpMsgPtr + pos, sizeof(uint16_t));
    pos += 2;
    id_ = ntohs(id_);
    quesCnt_ = ntohs(quesCnt_);
    ansCnt_ = ntohs(ansCnt_);
    authRecCnt_ = ntohs(authRecCnt_);
    addRecCnt_ = ntohs(addRecCnt_);
}
