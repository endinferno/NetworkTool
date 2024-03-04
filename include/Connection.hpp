#pragma once

#include <memory>
#include <string>

#include "PosixFd.hpp"

class Connection
{
public:
    explicit Connection(PosixFdPtr posixFd);
    void Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    virtual ~Connection() = default;

private:
    PosixFdPtr posixFd_;
    bool isConnect_ = false;
};

using ConnectionPtr = std::shared_ptr<Connection>;
