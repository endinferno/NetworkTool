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
        ssize_t readBytes = sslFd->Read(readBuf_);
        DEBUG("readBytes {}\n", readBytes);
        if (readBytes < 0) {
            int savedErr = sslFd->GetError(static_cast<int>(readBytes));
            if (savedErr == SSL_ERROR_WANT_READ) {
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
    auto conn = conn_.lock();
    conn->SetConnectStatus(true);
    ModEvent(chan, Pollable::READ_EVENT | Pollable::Event::EventEt);
    if (writeCompleteCallback_ != nullptr) {
        writeCompleteCallback_(chan);
    }
}

void SslClient::Write(const std::string& writeBuf)
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

void SslClient::SetWriteCompleteCallback(
    Client::WriteCompleteCallback&& callback)
{
    writeCompleteCallback_ = std::move(callback);
}

void SslClient::HandleNewConnection(ChannelPtr& chan)
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
