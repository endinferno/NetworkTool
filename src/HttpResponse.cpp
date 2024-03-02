#include "HttpResponse.hpp"
#include "fmt/core.h"

std::string HttpResponse::GetVersion() const
{
    return version_;
}

void HttpResponse::SetVersion(const std::string& version)
{
    version_ = version;
}

std::string HttpResponse::GetHeader(const std::string& header) const
{
    auto headerIt = headerMap_.find(header);
    if (headerIt == headerMap_.end()) {
        throw std::runtime_error(fmt::format("No such header {}", header));
    }
    return headerIt->second;
}

void HttpResponse::AddHeader(const std::string& key, const std::string& value)
{
    headerMap_.insert({ key, value });
}

HttpResponse::HttpStatusCode HttpResponse::GetStatus() const
{
    return statusCode_;
}

void HttpResponse::SetStatus(const HttpResponse::HttpStatusCode& status)
{
    statusCode_ = status;
}

std::string HttpResponse::GetBody() const
{
    return body_;
}

void HttpResponse::SetBody(const std::string& body)
{
    body_ = body;
}

void HttpResponse::Clear()
{
    version_.clear();
    statusCode_ = HttpStatusCode::UNKNOWN;
    headerMap_.clear();
    body_.clear();
}
