#pragma once

#include <memory>

#include <openssl/ssl.h>

#include "Socket/Socket.hpp"

class SslWrapper
{
public:
    SslWrapper();
    void SetFd(SocketPtr sock);
    void SetConnectState();
    int ShakeHands();
    int GetError(int shakeHandRet);
    ~SslWrapper();

private:
    SSL_CTX* sslContext_;
    SSL* sslHandle_;
    SocketPtr sock_;
};

using SslWrapperPtr = std::unique_ptr<SslWrapper>;
