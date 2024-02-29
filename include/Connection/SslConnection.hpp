#pragma once

#include "Connection/Connection.hpp"
#include "Utils/SslFd.hpp"

class SslConnection : public Connection
{
public:
    explicit SslConnection(SslFdPtr sslFd);
    void Write(const std::string& writeBuf) override;
    ~SslConnection() override = default;

private:
    SslFdPtr sslFd_;
};
