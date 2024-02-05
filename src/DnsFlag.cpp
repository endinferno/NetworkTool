#include "DnsFlag.hpp"

DnsFlag::DnsFlag(uint16_t flag)
{
    Decode(flag);
}

void DnsFlag::SetQR(DnsBaseType::QRFlag qrFlag)
{
    QRFlag_ = qrFlag;
}

void DnsFlag::SetOpCode(DnsBaseType::OpCodeFlag opcodeFlag)
{
    opCode_ = opcodeFlag;
}

void DnsFlag::SetAA(bool aaFlag)
{
    AA_ = aaFlag;
}

void DnsFlag::SetTC(bool tcFlag)
{
    TC_ = tcFlag;
}

void DnsFlag::SetRD(bool rdFlag)
{
    RD_ = rdFlag;
}

void DnsFlag::SetRA(bool raFlag)
{
    RA_ = raFlag;
}

void DnsFlag::SetRespCode(DnsBaseType::RespCodeFlag respCodeFlag)
{
    respCode_ = respCodeFlag;
}

DnsBaseType::QRFlag DnsFlag::GetQR() const
{
    return QRFlag_;
}

DnsBaseType::OpCodeFlag DnsFlag::GetOpCode() const
{
    return opCode_;
}

bool DnsFlag::GetAA() const
{
    return AA_;
}

bool DnsFlag::GetTC() const
{
    return TC_;
}

bool DnsFlag::GetRD() const
{
    return RD_;
}

bool DnsFlag::GetRA() const
{
    return RA_;
}

DnsBaseType::RespCodeFlag DnsFlag::GetRespCode() const
{
    return respCode_;
}

std::string DnsFlag::Stringify() const
{
    uint16_t dnsFlag = htons(Encode());
    std::string content(sizeof(uint16_t), '0');
    ::memcpy(content.data(), &dnsFlag, sizeof(uint16_t));
    return content;
}

void DnsFlag::Parse(const std::string& udpMsg, size_t& pos)
{
    char* udpMsgPtr = const_cast<char*>(udpMsg.data() + pos);
    uint16_t flag = 0;
    ::memcpy(&flag, udpMsgPtr, sizeof(uint16_t));
    pos += 2;
    flag = ntohs(flag);
    Decode(flag);
}

void DnsFlag::Decode(uint16_t flag)
{
    QRFlag_ = static_cast<DnsBaseType::QRFlag>((flag >> 15) & 0x1);
    opCode_ = static_cast<DnsBaseType::OpCodeFlag>((flag >> 11) & 0xF);
    AA_ = static_cast<bool>((flag >> 10) & 0x1);
    TC_ = static_cast<bool>((flag >> 9) & 0x1);
    RD_ = static_cast<bool>((flag >> 8) & 0x1);
    RA_ = static_cast<bool>((flag >> 7) & 0x1);
    respCode_ = static_cast<DnsBaseType::RespCodeFlag>(flag & 0xF);
}

uint16_t DnsFlag::Encode() const
{
    uint16_t dnsFlag = 0;
    dnsFlag |= (static_cast<uint16_t>(QRFlag_) & 0x1) << 15;
    dnsFlag |= (static_cast<uint16_t>(opCode_) & 0xF) << 11;
    dnsFlag |= (static_cast<uint16_t>(AA_) & 0x1) << 10;
    dnsFlag |= (static_cast<uint16_t>(TC_) & 0x1) << 9;
    dnsFlag |= (static_cast<uint16_t>(RD_) & 0x1) << 8;
    dnsFlag |= (static_cast<uint16_t>(RA_) & 0x1) << 7;
    dnsFlag |= (static_cast<uint16_t>(respCode_) & 0xF);
    return dnsFlag;
}
