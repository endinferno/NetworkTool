#pragma once

#include "Utils/SslFd.hpp"

class SslConnection
{
public:
    explicit SslConnection(SslFdPtr sslFd);
    void Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~SslConnection() = default;

private:
    SslFdPtr sslFd_;
    bool isConnect_;
};

using SslConnectionPtr = std::shared_ptr<SslConnection>;
