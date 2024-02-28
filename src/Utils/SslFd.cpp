#include "Utils/SslFd.hpp"

SslFd::SslFd(SocketPtr& sock)
    : PosixFd(sock->GetFd())
    , sock_(sock)
{
    SSL_load_error_strings();
    SSL_library_init();
    sslContext_ = ::SSL_CTX_new(SSLv23_method());
    sslHandle_ = ::SSL_new(sslContext_);
}

void SslFd::SetFd()
{
    int ret = ::SSL_set_fd(sslHandle_, GetFd());
    if (ret == 0) {
        throw std::runtime_error("Fail to set ssl fd\n");
    }
}

void SslFd::SetConnectState()
{
    ::SSL_set_connect_state(sslHandle_);
}

int SslFd::ShakeHands()
{
    return ::SSL_do_handshake(sslHandle_);
}

int SslFd::GetError(int shakeHandRet)
{
    return ::SSL_get_error(sslHandle_, shakeHandRet);
}

SslFd::~SslFd()
{
    ::SSL_shutdown(sslHandle_);
    ::SSL_free(sslHandle_);
    ::SSL_CTX_free(sslContext_);
}
