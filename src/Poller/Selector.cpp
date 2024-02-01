#include "Poller/Selector.hpp"
#include "Logger.hpp"

TcpChannels Selector::PollEvent()
{
    int maxFd = -1;
    fd_set readFds;
    fd_set writeFds;
    fd_set exceptFds;
    struct timeval intervalTime;
    std::vector<TcpChannel*> tcpChans;

    FillIntervalTime(intervalTime);
    FillFds(&readFds, &writeFds, &exceptFds, maxFd);

    int eventCnt =
        ::select(maxFd + 1, &readFds, &writeFds, &exceptFds, &intervalTime);
    DEBUG("eventCnt {}\n", eventCnt);
    if (eventCnt == -1) {
        throw std::runtime_error(
            fmt::format("Fail to poll event {} {}\n", eventCnt, errno));
    } else if (eventCnt == 0) {
        // Timeout rings
        return tcpChans;
    }
    for (auto& [fd, eventChan] : eventChanMap_) {
        auto tcpChan = eventChan.second;
        uint32_t event = 0;
        if (FD_ISSET(fd, &readFds)) {
            event |= Pollable::Event::EventIn;
        }
        if (FD_ISSET(fd, &writeFds)) {
            event |= Pollable::Event::EventOut;
        }
        if (FD_ISSET(fd, &exceptFds)) {
            event |= Pollable::Event::EventErr;
        }
        if (event == 0) {
            continue;
        }
        tcpChan->SetEvent(event);
        tcpChans.emplace_back(tcpChan);
    }
    return tcpChans;
}

void Selector::EventCtl(TcpChannel* tcpChan, enum EventCtl opera,
                        uint32_t event)
{
    int sockFd = tcpChan->GetSock()->GetFd();
    switch (opera) {
    case Pollable::EventCtl::Add:
    {
        eventChanMap_[sockFd] = { event, tcpChan };
        break;
    }
    case Pollable::EventCtl::Del:
    {
        eventChanMap_.erase(sockFd);
        break;
    }
    case Pollable::EventCtl::Mod:
    {
        eventChanMap_[sockFd].first = event;
        break;
    }
    default:
        throw std::runtime_error(
            fmt::format("No such operation {}\n", static_cast<int>(opera)));
    }
}

void Selector::FillFds(fd_set* readFds, fd_set* writeFds, fd_set* exceptFds,
                       int& maxFd)
{
    EmptyFds(readFds, writeFds, exceptFds);
    for (auto& [fd, eventChan] : eventChanMap_) {
        maxFd = std::max(maxFd, fd);
        uint32_t event = eventChan.first;
        if ((event & Pollable::Event::EventIn) != 0U) {
            FD_SET(fd, readFds);
        }
        if ((event & Pollable::Event::EventOut) != 0U) {
            FD_SET(fd, writeFds);
        }
        if ((event & Pollable::Event::EventErr) != 0U) {
            FD_SET(fd, exceptFds);
        }
    }
}

void Selector::EmptyFds(fd_set* readFds, fd_set* writeFds, fd_set* exceptFds)
{
    FD_ZERO(readFds);
    FD_ZERO(writeFds);
    FD_ZERO(exceptFds);
}

void Selector::FillIntervalTime(struct timeval& intervalTime)
{
    intervalTime.tv_sec = EVENT_POLLER_TIMEOUT / 1000;
    intervalTime.tv_usec = static_cast<int64_t>(EVENT_POLLER_TIMEOUT) * 1000;
}
