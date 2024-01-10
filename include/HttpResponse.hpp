#pragma once

#include <string>
#include <unordered_map>

class HttpResponse
{
public:
    HttpResponse() = default;
    std::string GetVersion() const;
    std::string GetHeader(const std::string& header) const;
    int GetStatus() const;
    std::string GetBody() const;
    void Parse(const std::string& httpResp);
    ~HttpResponse() = default;

private:
    void ParseStatusLine(const std::string& line);
    void ParseHeader(const std::string& line);

private:
    std::string version_;
    int statusCode_;
    std::unordered_map<std::string, std::string> headerMap_;
    std::string body_;
};
