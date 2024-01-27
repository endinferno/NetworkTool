#include <stdexcept>

#include "SslWrapper.hpp"

SslWrapper::SslWrapper()
{
    SSL_load_error_strings();
    SSL_library_init();
    sslContext_ = SSL_CTX_new(SSLv23_method());
    sslHandle_ = SSL_new(sslContext_);
}

void SslWrapper::SetFd(int fd)
{
    int ret = SSL_set_fd(sslHandle_, fd);
    if (ret == 0) {
        throw std::runtime_error("Fail to set ssl fd\n");
    }
}

void SslWrapper::SetConnectState()
{
    SSL_set_connect_state(sslHandle_);
}

int SslWrapper::ShakeHands()
{
    return SSL_do_handshake(sslHandle_);
}

int SslWrapper::GetError(int r)
{
    return SSL_get_error(sslHandle_, r);
}

SslWrapper::~SslWrapper()
{
    SSL_shutdown(sslHandle_);
    SSL_free(sslHandle_);
    SSL_CTX_free(sslContext_);
}
