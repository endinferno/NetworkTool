#include "Client/Client.hpp"
#include "Connector/ConnectorFactory.hpp"
#include "Utils/Logger.hpp"

Client::Client(EventPollerPtr& poller, Connector::ConnectorType connectorType)
    : EpollHandler(poller)
    , connectorType_(connectorType)
    , connector_(ConnectorFactory::Create(poller, connectorType))
    , readBuf_(MAX_READ_BUFFER, 0)
{}

void Client::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to handle client\n");
}

void Client::HandleReadEvent(ChannelPtr&& chan)
{
    DEBUG("Handle read event\n");
    auto sock = std::dynamic_pointer_cast<Socket>(chan->GetFd());
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        ssize_t readBytes = sock->Read(readBuf_);
        int savedErr = sock->GetErrno();
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
        DEBUG("Call message callback\n");
        if (messageCallback_ != nullptr) {
            // TODO: maybe there is a better way
            readBuf_.resize(readBytes);
            messageCallback_(chan, readBuf_);
        }
    }
}

void Client::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Handle write event\n");
    auto conn = conn_.lock();
    conn->SetConnectStatus(true);
    ModEvent(chan, Pollable::READ_EVENT | Pollable::Event::EventEt);
    if (writeCompleteCallback_ != nullptr) {
        writeCompleteCallback_(chan);
    }
}

void Client::Write(const std::string& writeBuf)
{
    if (conn_.expired()) {
        DEBUG("The connection has expired\n");
        return;
    }
    auto conn = conn_.lock();
    if (conn->GetConnectStatus()) {
        conn->Write(writeBuf);
    }
}

void Client::Connect(const IPAddress& serverIp, const uint16_t& serverPort)
{
    connector_->SetNewConnectionCallback(
        [this](ChannelPtr& chan) { HandleNewConnection(chan); });
    connector_->Connect(serverIp, serverPort);
}

void Client::Shutdown(ChannelPtr& chan)
{
    DelEvent(chan);
}

void Client::SetMessageCallback(MessageCallback&& callback)
{
    messageCallback_ = std::move(callback);
}

void Client::SetWriteCompleteCallback(WriteCompleteCallback&& callback)
{
    writeCompleteCallback_ = std::move(callback);
}

void Client::HandleNewConnection(ChannelPtr& chan)
{
    INFO("New connection construct\n");
    auto conn = std::make_shared<Connection>(chan->GetFd());

    conn_ = conn;
    chan->SetConnection(conn);
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    ModEvent(chan, Pollable::WRITE_EVENT | Pollable::Event::EventEt);
}
