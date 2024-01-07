#include <cassert>

#include "Logger.hpp"
#include "TcpConnector.hpp"

TcpConnector::TcpConnector(std::shared_ptr<Epoller>& epoller)
    : EpollHandler(epoller)
{}

void TcpConnector::HandleErrorEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    ERROR("Fail to connect\n");
}

void TcpConnector::HandleReadEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    assert(true);
}

void TcpConnector::HandleWriteEvent(std::shared_ptr<TcpChannel> tcpChan)
{
    auto tcpSock = tcpChan->GetSock();
    int opt = tcpSock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        throw std::runtime_error(fmt::format("Fail to connect {}", opt));
    }
    INFO("Success to connect\n");
    epoller_->DelEvent(tcpChan);
    if (callback_ != nullptr) {
        callback_(tcpChan);
    }
}

void TcpConnector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

void TcpConnector::Connect(std::shared_ptr<TcpSocket>& tcpSock)
{
    tcpSock->Connect("hq.sinajs.cn", 80);

    auto tcpChan = std::make_shared<TcpChannel>(tcpSock);
    tcpChan->SetReadCallback(
        std::bind(&TcpConnector::HandleReadEvent, this, std::placeholders::_1));
    tcpChan->SetWriteCallback(std::bind(
        &TcpConnector::HandleWriteEvent, this, std::placeholders::_1));
    tcpChan->SetErrorCallback(std::bind(
        &TcpConnector::HandleErrorEvent, this, std::placeholders::_1));

    epoller_->AddEvent(tcpChan, EPOLLOUT | EPOLLET);
}
