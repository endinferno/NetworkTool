#pragma once

#include <openssl/ssl.h>

#include "PosixFd.hpp"
#include "Socket/Socket.hpp"

class SslFd : public PosixFd
{
public:
    explicit SslFd(SocketPtr& sock);
    void SetFd();
    void SetConnectState();
    int ShakeHands();
    int GetError(int shakeHandRet);
    int Write(const std::string& writeBuf);
    int Read(std::string& readBuf);
    ~SslFd() override;

private:
    SSL_CTX* sslContext_ = nullptr;
    SSL* sslHandle_ = nullptr;
    SocketPtr sock_ = nullptr;
};

using SslFdPtr = std::shared_ptr<SslFd>;
