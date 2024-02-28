#pragma once

#include "Socket/Socket.hpp"

class Connection
{
public:
    explicit Connection(SocketPtr sock);
    void Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~Connection() = default;

private:
    SocketPtr sock_;
    bool isConnect_;
};

using ConnectionPtr = std::shared_ptr<Connection>;
