#pragma once

#include <string>
#include <unordered_map>

class HttpRequest
{
public:
    enum class ReqType
    {
        GET,
        POST,
        UNKNOWN,
    };

    HttpRequest();
    void SetReqType(const ReqType& req);
    void SetUrl(const std::string& url);
    void AddHeader(const std::string& key, const std::string& value);
    void AddParam(const std::string& key, const std::string& value);
    std::string Stringify() const;
    ~HttpRequest() = default;

private:
    std::string GetReqType() const;
    std::string GetReqParam() const;
    std::string GetReqLine() const;
    std::string GetReqHeader() const;
    std::string GetReqData() const;

    std::string version_ = "1.1";
    ReqType reqType_;
    std::string url_;
    std::unordered_map<std::string, std::string> paramMap_;
    std::unordered_map<std::string, std::string> headerMap_;
};
