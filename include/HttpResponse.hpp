#pragma once

#include <string>
#include <unordered_map>

class HttpResponse
{
public:
    enum HttpStatusCode
    {
        OK = 200,
        UNKNOWN = -1,
    };
    HttpResponse() = default;
    std::string GetVersion() const;
    void SetVersion(const std::string& version);
    std::string GetHeader(const std::string& header) const;
    void AddHeader(const std::string& key, const std::string& value);
    HttpStatusCode GetStatus() const;
    void SetStatus(const HttpStatusCode& status);
    std::string GetBody() const;
    void SetBody(const std::string& body);
    void Clear();
    ~HttpResponse() = default;

private:
    void ParseStatusLine(const std::string& line);
    void ParseHeader(const std::string& line);

    std::string version_;
    HttpStatusCode statusCode_;
    std::unordered_map<std::string, std::string> headerMap_;
    std::string body_;
};
