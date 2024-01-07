#include "TcpClient.hpp"
#include "Logger.hpp"

TcpClient::TcpClient(std::shared_ptr<Epoller>& epoller)
    : EpollHandler(epoller)
    , tcpConnector_(epoller)
    , tcpConn_(nullptr)
    , readBuf_(MAX_READ_BUFFER)
    , isWritable(false)
{}

void TcpClient::HandleErrorEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    ERROR("Fail to handle client\n");
}

void TcpClient::HandleReadEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    auto tcpSock = tcpChan->GetSock();
    while (true) {
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
        DEBUG("Read Bytes {}\n", readBytes);
        for (ssize_t i = 0; i < readBytes; i++) {
            INFO("{}", readBuf_[i]);
        }
        INFO("\n");
    }
}

void TcpClient::HandleWriteEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    isWritable = true;
}

ssize_t TcpClient::Write(const std::vector<char>& writeBuf)
{
    if (isWritable == false) {
        return 0;
    }
    return tcpConn_->Write(writeBuf);
}

void TcpClient::Connect(const std::string& domainName, uint16_t port)
{
    auto tcpSock_ = std::make_shared<TcpSocket>();
    tcpSock_->SetReuseAddr();
    tcpSock_->SetReusePort();
    tcpSock_->SetNonBlock();
    tcpConnector_.SetNewConnectionCallback(std::bind(
        &TcpClient::HandleNewConnection, this, std::placeholders::_1));
    tcpConnector_.Connect(tcpSock_, domainName, port);
}

void TcpClient::HandleNewConnection(std::shared_ptr<TcpChannel> tcpChan)
{
    INFO("New connection construct\n");
    tcpConn_ = std::make_shared<TcpConnection>(tcpChan->GetSock());
    tcpConn_->SetConnectStatus(true);

    tcpChan->SetReadCallback(
        std::bind(&TcpClient::HandleReadEvent, this, std::placeholders::_1));
    tcpChan->SetWriteCallback(
        std::bind(&TcpClient::HandleWriteEvent, this, std::placeholders::_1));
    tcpChan->SetErrorCallback(
        std::bind(&TcpClient::HandleErrorEvent, this, std::placeholders::_1));

    epoller_->AddEvent(tcpChan, EPOLLIN | EPOLLOUT | EPOLLET);
}
