#include "TcpConnector.hpp"
#include "Logger.hpp"

TcpConnector::TcpConnector(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

void TcpConnector::HandleErrorEvent([[maybe_unused]] TcpChannelPtr tcpChan)
{
    ERROR("Fail to connect\n");
}

void TcpConnector::HandleReadEvent([[maybe_unused]] TcpChannelPtr tcpChan)
{
    static_assert(true);
}

void TcpConnector::HandleWriteEvent(TcpChannelPtr tcpChan)
{
    auto tcpSock = tcpChan->GetSock();
    int opt = tcpSock->GetSockOpt(SOL_SOCKET, SO_ERROR);
    if (opt != 0) {
        throw std::runtime_error(fmt::format("Fail to connect {}", opt));
    }
    INFO("Success to connect\n");
    poller_->DelEvent(tcpChan);
    if (callback_ != nullptr) {
        callback_(tcpChan);
    }
}

void TcpConnector::SetNewConnectionCallback(NewConnectionCallback callback)
{
    callback_ = std::move(callback);
}

void TcpConnector::Connect(const std::string& domainName, uint16_t port)
{
    auto tcpSock = std::make_shared<TcpSocket>();
    tcpSock->SetReuseAddr();
    tcpSock->SetReusePort();
    tcpSock->SetNonBlock();
    tcpSock->Connect(domainName, port);

    auto tcpChan = std::make_shared<TcpChannel>(tcpSock);
    tcpChan->SetReadCallback(nullptr);
    tcpChan->SetWriteCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleWriteEvent(tcpChan); });
    tcpChan->SetErrorCallback(
        [this](TcpChannelPtr&& tcpChan) { HandleErrorEvent(tcpChan); });

#if defined(USE_EPOLL)
    poller_->AddEvent(tcpChan,
                      Pollable::Event::EventOut | Pollable::Event::EventEt);
#elif defined(USE_POLL)
    poller_->AddEvent(tcpChan, Pollable::Event::EventOut);
#elif defined(USE_SELECT)
    poller_->AddEvent(tcpChan, Pollable::Event::EventOut);
#endif
}
