#include <cstdio>
#include <sstream>

#include "HttpResponse.hpp"
#include "Utils/Logger.hpp"

std::string HttpResponse::GetVersion() const
{
    return version_;
}

std::string HttpResponse::GetHeader(const std::string& header) const
{
    auto headerIt = headerMap_.find(header);
    if (headerIt == headerMap_.end()) {
        throw std::runtime_error(fmt::format("No such header {}", header));
    }
    return headerIt->second;
}

int HttpResponse::GetStatus() const
{
    return statusCode_;
}

std::string HttpResponse::GetBody() const
{
    return body_;
}

void HttpResponse::ParseStatusLine(const std::string& line)
{
    version_.resize(3);
    std::sscanf(line.data(), "HTTP/%s %d \n", version_.data(), &statusCode_);
}

void HttpResponse::ParseHeader(const std::string& line)
{
    auto pos = line.find(':');
    if (pos == std::string::npos) {
        return;
    }
    std::string key(line.substr(0, pos));
    std::string value(line.substr(pos + 2, line.size() - (pos + 2)));
    headerMap_.insert({ key, value });
}

void HttpResponse::Parse(const std::string& httpResp)
{
    enum HttpPart
    {
        HttpStatusLine,
        HttpHeader,
        HttpBody,
    };
    HttpPart part = HttpStatusLine;
    std::string line;
    std::istringstream inputStream(httpResp);
    while (std::getline(inputStream, line)) {
        switch (part) {
        case HttpStatusLine:
        {
            ParseStatusLine(line);
            part = HttpHeader;
            break;
        }
        case HttpHeader:
        {
            if (line.size() == 1) {
                part = HttpBody;
                break;
            }
            ParseHeader(line);
            break;
        }
        case HttpBody:
        {
            body_.append(line);
            break;
        }
        default: throw std::runtime_error("No such HTTP part\n");
        }
    }
}
