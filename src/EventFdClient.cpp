#include "EventFdClient.hpp"
#include "Utils/Logger.hpp"
#include <memory>

EventFdClient::EventFdClient(EventPollerPtr& poller)
    : EpollHandler(poller)
{}

size_t EventFdClient::Create()
{
    std::lock_guard<std::mutex> lckGuard(createMtx_);
    auto eventFd = std::make_shared<EventFd>();
    DEBUG("Create eventfd {}\n", eventFd->GetFd());
    auto chan = std::make_shared<Channel>(eventFd);
    chan->SetErrorCallback(
        [this](ChannelPtr&& chan) { HandleErrorEvent(std::move(chan)); });
    chan->SetReadCallback(
        [this](ChannelPtr&& chan) { HandleReadEvent(std::move(chan)); });
    chan->SetWriteCallback(
        [this](ChannelPtr&& chan) { HandleWriteEvent(std::move(chan)); });
    AddEvent(chan,
             Pollable::WRITE_EVENT | Pollable::ERROR_EVENT |
                 Pollable::Event::EventEt);
    eventFd->SetWritable(false);
    eventFds_.emplace_back(std::move(eventFd));
    return eventFds_.size() - 1;
}

int EventFdClient::Write(size_t idx, const eventfd_t& event)
{
    if (!eventFds_[idx]->IsWritable()) {
        return -1;
    }
    eventFds_[idx]->SetWritable(false);
    return eventFds_[idx]->Write(event);
}

void EventFdClient::HandleErrorEvent([[maybe_unused]] ChannelPtr&& chan)
{
    ERROR("Fail to handle eventfd");
}

void EventFdClient::HandleReadEvent(ChannelPtr&& chan)
{
    DEBUG("HandleReadEvent\n");
    auto eventFd = std::dynamic_pointer_cast<EventFd>(chan->GetFd());
    eventfd_t event;
    int ret = eventFd->Read(event);
    if (ret == -1) {
        throw std::runtime_error("Dont have data in eventfd\n");
    }
    ModEvent(chan,
             Pollable::WRITE_EVENT | Pollable::ERROR_EVENT |
                 Pollable::Event::EventEt);
    if (readCallback_ != nullptr) {
        readCallback_(event);
    }
}

void EventFdClient::HandleWriteEvent([[maybe_unused]] ChannelPtr&& chan)
{
    DEBUG("HandleWriteEvent\n");
    auto eventFd = std::dynamic_pointer_cast<EventFd>(chan->GetFd());
    eventFd->SetWritable(true);
    ModEvent(chan,
             Pollable::READ_EVENT | Pollable::ERROR_EVENT |
                 Pollable::Event::EventEt);
}

void EventFdClient::SetReadCallback(ReadCallback&& callback)
{
    readCallback_ = std::move(callback);
}
