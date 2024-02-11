#pragma once

#include <netinet/in.h>

#include <vector>

#include "DnsAnswer.hpp"
#include "DnsHeader.hpp"
#include "DnsQuestion.hpp"
#include "Utils/Logger.hpp"

class DnsMessage
{
public:
    DnsMessage();
    void SetHeaderFlag(const DnsFlag& dnsFlag);
    [[nodiscard]] DnsHeader GetHeader() const;
    [[nodiscard]] std::vector<DnsQuestion> GetQuestion() const;
    [[nodiscard]] std::vector<DnsAnswer> GetAnswer() const;
    void AddDomain(const std::string& domainName);
    [[nodiscard]] std::string Stringify() const;
    void Parse(const std::string& udpMsg);

private:
    DnsHeader header_;
    std::vector<DnsQuestion> questions_;
    std::vector<DnsAnswer> answers_;
};

// NOLINTBEGIN(readability-identifier-naming)
template<>
struct fmt::formatter<DnsMessage>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }
    template<typename FormatContext>
    auto format(const DnsMessage& message, FormatContext& ctx)
    {
        auto header = message.GetHeader();
        auto questions = message.GetQuestion();
        auto answers = message.GetAnswer();
        std::string questionsStr;
        std::string answersStr;
        for (const auto& question : questions) {
            questionsStr = fmt::format("{} {}", questionsStr, question);
        }
        for (const auto& answer : answers) {
            DEBUG("{}\n", answer);
            answersStr = fmt::format("{} {}", answersStr, answer);
        }

        return fmt::format_to(
            ctx.out(), "{} {} {}", header, questionsStr, answersStr);
    }
};
// NOLINTEND(readability-identifier-naming)
