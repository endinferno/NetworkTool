#pragma once

#include <memory>
#include <string>

class Connection
{
public:
    Connection() = default;
    virtual void Write(const std::string& writeBuf) = 0;
    void SetConnectStatus(bool status)
    {
        isConnect_ = status;
    }
    [[nodiscard]] bool GetConnectStatus() const
    {
        return isConnect_;
    }
    virtual ~Connection() = default;

private:
    bool isConnect_ = false;
};

using ConnectionPtr = std::shared_ptr<Connection>;
