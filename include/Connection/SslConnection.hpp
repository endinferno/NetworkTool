#pragma once

#include "Connection/Connection.hpp"
#include "Utils/SslFd.hpp"

class SslConnection : public Connection
{
public:
    explicit SslConnection(SslFdPtr sslFd)
        : sslFd_(std::move(sslFd))
    {}
    void Write(const std::string& writeBuf) override
    {
        ssize_t writeBytes = sslFd_->Write(writeBuf);
        if (writeBytes != static_cast<ssize_t>(writeBuf.size())) {
            throw std::runtime_error("Fail to write to socket\n");
        }
    }
    ~SslConnection() override = default;

private:
    SslFdPtr sslFd_;
};
