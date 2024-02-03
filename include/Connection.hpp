#pragma once

#include "Socket/Socket.hpp"

class Connection
{
public:
    Connection();
    explicit Connection(SocketPtr& sock);
    void Bind(const SocketPtr& sock);
    void Write(const std::string& writeBuf);
    void SetConnectStatus(bool status);
    [[nodiscard]] bool GetConnectStatus() const;
    ~Connection() = default;

private:
    SocketPtr sock_;
    bool isConnect_;
};

using ConnectionPtr = std::shared_ptr<Connection>;
