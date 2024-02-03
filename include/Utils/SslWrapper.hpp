#pragma once

#include <openssl/ssl.h>

class SslWrapper
{
public:
    SslWrapper();
    void SetFd(int sockFd);
    void SetConnectState();
    int ShakeHands();
    int GetError(int shakeHandRet);
    ~SslWrapper();

private:
    SSL_CTX* sslContext_;
    SSL* sslHandle_;
};
