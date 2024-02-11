#include <stdexcept>

#include "HttpRequest.hpp"
#include "fmt/core.h"

HttpRequest::HttpRequest()
    : reqType_(ReqType::UNKNOWN)
{}

void HttpRequest::SetReqType(const ReqType& req)
{
    reqType_ = req;
}

void HttpRequest::SetUrl(const std::string& url)
{
    url_ = url;
}

void HttpRequest::AddHeader(const std::string& key, const std::string& value)
{
    headerMap_.insert({ key, value });
}

void HttpRequest::AddParam(const std::string& key, const std::string& value)
{
    paramMap_.insert({ key, value });
}

std::string HttpRequest::Stringify() const
{
    return fmt::format("{}{}\r\n", GetReqLine(), GetReqHeader());
}

std::string HttpRequest::GetReqType() const
{
    switch (reqType_) {
    case ReqType::GET:
    {
        return "GET";
    }
    case ReqType::POST:
    {
        return "POST";
    }
    case ReqType::UNKNOWN:
    {
        throw std::runtime_error("Wrong HTTP Request\n");
    }
    }
    throw std::runtime_error("Wrong HTTP Request\n");
}

std::string HttpRequest::GetReqParam() const
{
    size_t cnt = 0;
    std::string reqParam;
    for (const auto& [key, val] : paramMap_) {
        reqParam += fmt::format("{}={}", key, val);
        if (cnt != paramMap_.size() - 1) {
            reqParam.push_back('&');
        }
        cnt++;
    }
    return reqParam;
}

std::string HttpRequest::GetReqLine() const
{
    auto reqType = GetReqType();
    auto reqParam = GetReqParam();
    if (reqParam.empty()) {
        return fmt::format("{} {} HTTP/{}\r\n", reqType, url_, version_);
    } else {
        return fmt::format(
            "{} {}?{} HTTP/{}\r\n", reqType, url_, reqParam, version_);
    }
}

std::string HttpRequest::GetReqHeader() const
{
    std::string reqParam;
    for (const auto& [key, val] : headerMap_) {
        reqParam += fmt::format("{}: {}\r\n", key, val);
    }
    return reqParam;
}

std::string HttpRequest::GetReqData() const
{
    // TODO: implement in the future
    throw std::runtime_error("Not implemented yet!");
}
