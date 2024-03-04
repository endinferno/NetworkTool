#pragma once

#include <memory>

#include <unistd.h>

#include "Utils/NonCopyable.hpp"

class PosixFd : public NonCopyable
{
public:
    explicit PosixFd(int posixFd)
        : posixFd_(posixFd)
    {}
    virtual ssize_t Write(const std::string& writeBuf) = 0;
    virtual ssize_t Read(std::string& readBuf) = 0;
    [[nodiscard]] int GetFd() const
    {
        return posixFd_;
    }
    virtual ~PosixFd() = default;

private:
    int posixFd_;
};

using PosixFdPtr = std::shared_ptr<PosixFd>;
