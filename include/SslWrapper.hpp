#pragma once

#include <openssl/ssl.h>

class SslWrapper
{
public:
    SslWrapper();
    void SetFd(int fd);
    void SetConnectState();
    int ShakeHands();
    int GetError(int r);
    ~SslWrapper();

private:
    SSL_CTX* sslContext_;
    SSL* sslHandle_;
};
