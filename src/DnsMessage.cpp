#include <ctime>

#include "DnsMessage.hpp"

DnsMessage::DnsMessage()
{
    ::srand(::time(nullptr));
    header_.SetId(rand() % 0xFFFF);
    DEBUG("{:x}\n", header_.GetId());
}

void DnsMessage::SetHeaderFlag(const DnsFlag& dnsFlag)
{
    header_.SetFlag(dnsFlag);
}

DnsHeader DnsMessage::GetHeader() const
{
    return header_;
}

std::vector<DnsQuestion> DnsMessage::GetQuestion() const
{
    return questions_;
}

std::vector<DnsAnswer> DnsMessage::GetAnswer() const
{
    return answers_;
}

void DnsMessage::AddDomain(const std::string& domainName)
{
    header_.SetQuestionCount(header_.GetQuestionCount() + 1);
    DnsQuestion question;
    question.SetDomain(domainName);
    question.SetType(DnsBaseType::A);
    question.SetClass(DnsBaseType::Internet);
    questions_.emplace_back(question);
}

std::string DnsMessage::Stringify() const
{
    std::string content;
    content.append(header_.Stringify());
    for (const auto& question : questions_) {
        content.append(question.Stringify());
    }
    return content;
}

void DnsMessage::Parse(const std::string& udpMsg)
{
    for (auto& udpMsgCh : udpMsg) {
        INFO("0x{:02X} ", static_cast<unsigned char>(udpMsgCh));
    }
    INFO("\n\n");
    size_t pos = 0;
    header_.Parse(udpMsg, pos);
    uint8_t quesCnt = header_.GetQuestionCount();
    for (uint8_t i = 0; i < quesCnt; i++) {
        DnsQuestion question;
        question.Parse(udpMsg, pos);
        questions_.emplace_back(question);
    }
    uint8_t ansCnt = header_.GetAnswerCount();
    for (uint8_t i = 0; i < ansCnt; i++) {
        DnsAnswer answer;
        answer.Parse(udpMsg, pos);
        answers_.emplace_back(answer);
    }
}
