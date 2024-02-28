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
    [[nodiscard]] int GetFd() const
    {
        return posixFd_;
    }
    virtual ~PosixFd() = default;

private:
    int posixFd_;
};

using PosixFdPtr = std::shared_ptr<PosixFd>;
