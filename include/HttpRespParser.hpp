#pragma once

#include <optional>
#include <string_view>

#include "HttpResponse.hpp"

class HttpRespParser
{
public:
    HttpRespParser() = default;
    std::optional<HttpResponse> Parse(const std::string& httpMsg);
    ~HttpRespParser() = default;

private:
    std::optional<std::string_view> ParseLine(
        const std::string_view& httpMsgView, size_t& idx);
    void ParseStatusLine(const std::string_view& httpMsgView);
    void ParseHeader(const std::string_view& httpMsgView);
    void ParseBody(const std::string_view& httpMsgView);
    void Clear();

    enum ParsePhase
    {
        StatusLine,
        Header,
        Body,
        Finish,
    };
    ParsePhase parsePhase_ = ParsePhase::StatusLine;
    HttpResponse httpResp_;
    std::string savedHttpMsg_;
};
