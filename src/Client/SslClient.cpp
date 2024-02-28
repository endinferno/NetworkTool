#include "Client/SslClient.hpp"
#include "Utils/Logger.hpp"

SslClient::SslClient(EventPollerPtr& poller)
    : EpollHandler(poller)
    , connector_(poller)
{}

void SslClient::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to handle client\n");
}

void SslClient::HandleReadEvent(ChannelPtr&& chan)
{
    DEBUG("Handle read event\n");
    auto sslFd = std::dynamic_pointer_cast<SslFd>(chan->GetFd());
    while (true) {
        // TODO: maybe there is a better way
        readBuf_.resize(MAX_READ_BUFFER);
        int readBytes = sslFd->Read(readBuf_);
        DEBUG("readBytes {}\n", readBytes);
        if (readBytes < 0) {
            int savedErr = sslFd->GetError(readBytes);
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

void SslClient::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("Handle write event\n");
    conn_->SetConnectStatus(true);
    ModEvent(chan, Pollable::READ_EVENT | Pollable::Event::EventEt);
}

void SslClient::Write(const std::string& writeBuf)
{
    if (conn_ == nullptr || !conn_->GetConnectStatus()) {
        return;
    }
    conn_->Write(writeBuf);
}

void SslClient::Connect(const IPAddress& serverIp, const uint16_t& serverPort)
{
    connector_.SetNewConnectionCallback(
        [this](ChannelPtr& chan) { HandleNewConnection(chan); });
    connector_.Connect(serverIp, serverPort);
}

void SslClient::Shutdown(ChannelPtr& chan)
{
    DelEvent(chan);
}

void SslClient::SetMessageCallback(Client::MessageCallback&& callback)
{
    messageCallback_ = std::move(callback);
}

void SslClient::SetConnectCallback(Client::ConnectCallback&& callback)
{
    connectCallback_ = std::move(callback);
}

void SslClient::HandleNewConnection(ChannelPtr& chan)
{
    INFO("New connection construct\n");
    conn_ = std::make_shared<SslConnection>(
        std::dynamic_pointer_cast<SslFd>(chan->GetFd()));

    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });

    ModEvent(chan, Pollable::WRITE_EVENT | Pollable::Event::EventEt);
    DEBUG("Call connect done callback\n");
    if (connectCallback_ != nullptr) {
        connectCallback_(chan);
    }
}
