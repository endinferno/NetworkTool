#include <cassert>

#include "Logger.hpp"
#include "TcpConnector.hpp"

TcpConnector::TcpConnector(std::shared_ptr<Epoller>& epoller)
    : EpollHandler(epoller)
{}

void TcpConnector::HandleErrorEvent(std::shared_ptr<TcpConnection>& tcpConn)
{
    ERROR("Fail to connect\n");
}

void TcpConnector::HandleReadEvent(std::shared_ptr<TcpConnection>& tcpConn)
{
    assert(true);
}

void TcpConnector::HandleWriteEvent(std::shared_ptr<TcpConnection>& tcpConn)
{
    auto tcpSock = tcpConn->GetSock();
    int opt = tcpSock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        throw std::runtime_error(fmt::format("Fail to connect {}", opt));
    }
    INFO("Success to connect\n");
    epoller_->DelEvent(tcpConn);
}

std::shared_ptr<TcpConnection> TcpConnector::Connect(
    std::shared_ptr<TcpSocket>& tcpSock)
{
    tcpSock->SetReuseAddr();
    tcpSock->SetReusePort();
    tcpSock->SetNonBlock();
    tcpSock->Connect("hq.sinajs.cn", 80);

    auto tcpConn = std::make_shared<TcpConnection>(tcpSock);
    tcpConn->SetReadCallback(
        std::bind(&TcpConnector::HandleReadEvent, this, tcpConn));
    tcpConn->SetWriteCallback(
        std::bind(&TcpConnector::HandleWriteEvent, this, tcpConn));
    tcpConn->SetErrorCallback(
        std::bind(&TcpConnector::HandleErrorEvent, this, tcpConn));

    epoller_->AddEvent(tcpConn, EPOLLOUT | EPOLLET);
    return tcpConn;
}
