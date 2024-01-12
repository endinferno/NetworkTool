#include "TcpClient.hpp"
#include "Logger.hpp"

TcpClient::TcpClient(EpollerPtr& epoller)
    : EpollHandler(epoller)
    , tcpConnector_(epoller)
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void TcpClient::HandleErrorEvent(TcpChannelPtr tcpChan)
{
    ERROR("Fail to handle client\n");
}

void TcpClient::HandleReadEvent(TcpChannelPtr tcpChan)
{
    auto tcpSock = tcpChan->GetSock();
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        ssize_t readBytes = tcpSock->Read(readBuf_);
        int savedErr = tcpSock->GetErrno();
        if (readBytes == -1) {
            if (savedErr == EINTR || savedErr == EWOULDBLOCK ||
                savedErr == EAGAIN) {
                break;
            } else {
                ERROR("Fail to read {}\n", savedErr);
                break;
            }
        } else if (readBytes == 0) {
            ERROR("Connection closed\n");
            break;
        }
        if (callback_ != nullptr) {
            // TODO: maybe there is a better way
            readBuf_.resize(readBytes);
            callback_(readBuf_);
        }
    }
}

void TcpClient::HandleWriteEvent(TcpChannelPtr tcpChan)
{
    tcpConn_.SetConnectStatus(true);
}

void TcpClient::Write(const std::string& writeBuf)
{
    if (tcpConn_.GetConnectStatus() == false) {
        return;
    }
    tcpConn_.Write(writeBuf);
}

void TcpClient::Connect(const std::string& domainName, uint16_t port)
{
    tcpConnector_.SetNewConnectionCallback(std::bind(
        &TcpClient::HandleNewConnection, this, std::placeholders::_1));
    tcpConnector_.Connect(domainName, port);
}

void TcpClient::SetOnMessageCallback(OnMessageCallback callback)
{
    callback_ = std::move(callback);
}

void TcpClient::HandleNewConnection(TcpChannelPtr tcpChan)
{
    INFO("New connection construct\n");
    tcpConn_.Bind(tcpChan->GetSock());
    tcpConn_.SetConnectStatus(true);

    tcpChan->SetReadCallback(
        std::bind(&TcpClient::HandleReadEvent, this, std::placeholders::_1));
    tcpChan->SetWriteCallback(
        std::bind(&TcpClient::HandleWriteEvent, this, std::placeholders::_1));
    tcpChan->SetErrorCallback(
        std::bind(&TcpClient::HandleErrorEvent, this, std::placeholders::_1));

    epoller_->AddEvent(tcpChan, EPOLLIN | EPOLLOUT | EPOLLET);
}
