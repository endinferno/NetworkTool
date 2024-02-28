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
    conn_->SetConnectStatus(true);
}

void Client::Write(const std::string& writeBuf)
{
    if (conn_ == nullptr || !conn_->GetConnectStatus()) {
        return;
    }
    conn_->Write(writeBuf);
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

void Client::SetConnectCallback(ConnectCallback&& callback)
{
    connectCallback_ = std::move(callback);
}

void Client::HandleNewConnection(ChannelPtr& chan)
{
    INFO("New connection construct\n");
    conn_ = std::make_shared<Connection>();
    conn_->Bind(std::dynamic_pointer_cast<Socket>(chan->GetFd()));
    conn_->SetConnectStatus(true);

    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    AddEvent(chan,
             Pollable::Event::EventIn | Pollable::Event::EventOut |
                 Pollable::Event::EventEt);
    DEBUG("Call connect done callback\n");
    if (connectCallback_ != nullptr) {
        connectCallback_(chan);
    }
}
