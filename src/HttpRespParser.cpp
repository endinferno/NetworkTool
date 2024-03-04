#include "HttpRespParser.hpp"
#include "Utils/Logger.hpp"

std::optional<HttpResponse> HttpRespParser::Parse(const std::string& httpMsg)
{
    size_t idx = 0;
    std::string msg = savedHttpMsg_ + httpMsg;
    std::string_view httpMsgView(msg);
    while (true) {
        switch (parsePhase_) {
        case ParsePhase::StatusLine:
        {
            auto lineView = ParseLine(httpMsgView, idx);
            if (!lineView.has_value()) {
                savedHttpMsg_ = msg.substr(idx, msg.size() - idx);
                return std::nullopt;
            }
            ParseStatusLine(lineView.value());
            break;
        }
        case ParsePhase::Header:
        {
            auto lineView = ParseLine(httpMsgView, idx);
            if (!lineView.has_value()) {
                savedHttpMsg_ = msg.substr(idx, msg.size() - idx);
                return std::nullopt;
            }
            ParseHeader(lineView.value());
            break;
        }
        case ParsePhase::Body:
        {
            auto leftMsgView =
                httpMsgView.substr(idx, httpMsgView.size() - idx);
            ParseBody(leftMsgView);
            if (parsePhase_ == ParsePhase::Body) {
                savedHttpMsg_ = msg.substr(idx, msg.size() - idx);
                return std::nullopt;
            }
            break;
        }
        case ParsePhase::Finish:
        {
            HttpResponse retHttpResp = httpResp_;
            Clear();
            return retHttpResp;
        }
        default:
        {
            throw std::runtime_error("No such http parse phase");
        }
        }
    }
    return std::nullopt;
}

std::optional<std::string_view> HttpRespParser::ParseLine(
    const std::string_view& httpMsgView, size_t& idx)
{
    auto msgIter = httpMsgView.find("\r\n", idx);
    if (msgIter == std::string_view::npos) {
        return std::nullopt;
    } else {
        size_t oldIdx = idx;
        idx += msgIter - oldIdx;
        idx += 2;
        return httpMsgView.substr(oldIdx, msgIter - oldIdx);
    }
}

void HttpRespParser::ParseStatusLine(const std::string_view& httpMsgView)
{
    std::string version(3, 0);
    HttpResponse::HttpStatusCode status;
    std::sscanf(httpMsgView.data(),
                "HTTP/%s %d \n",
                version.data(),
                reinterpret_cast<int*>(&status));
    httpResp_.SetVersion(version);
    httpResp_.SetStatus(status);
    DEBUG("Version {}\n", version);
    DEBUG("Status {}\n", static_cast<int>(status));
    parsePhase_ = ParsePhase::Header;
}

void HttpRespParser::ParseHeader(const std::string_view& httpMsgView)
{
    auto msgIter = httpMsgView.find(':');
    if (msgIter == std::string_view::npos) {
        parsePhase_ = ParsePhase::Body;
        return;
    }
    std::string key(httpMsgView.substr(0, msgIter));
    std::string value(
        httpMsgView.substr(msgIter + 2, httpMsgView.size() - (msgIter + 2)));
    httpResp_.AddHeader(key, value);
    DEBUG("{} {}\n", key, value);
}

void HttpRespParser::ParseBody(const std::string_view& httpMsgView)
{
    auto contentLenStr = httpResp_.GetHeader("Content-Length");
    size_t contentLen = ::strtoul(contentLenStr.data(), nullptr, 10);
    DEBUG("{}\n", httpMsgView);
    DEBUG("Content Len {} Size {}\n", contentLen, httpMsgView.size());
    if (httpMsgView.size() == contentLen) {
        httpResp_.SetBody(std::string(httpMsgView));
        parsePhase_ = ParsePhase::Finish;
    } else {
        savedHttpMsg_ = std::string(httpMsgView);
    }
}

void HttpRespParser::Clear()
{
    parsePhase_ = ParsePhase::StatusLine;
    savedHttpMsg_.clear();
    auto retHttpResp = httpResp_;
    httpResp_.Clear();
}
