#pragma once

#include "fmt/core.h"

#define INFO_ENABLE 1
#ifdef NDEBUG
#    define DEBUG_ENABLE 0
#else
#    define DEBUG_ENABLE 1
#endif
#define ERROR_ENABLE 1

#if INFO_ENABLE
#    define INFO(format, ...)                  \
        do {                                   \
            fmt::print(format, ##__VA_ARGS__); \
        } while (0)
#else
#    define INFO(format, ...) \
        do {                  \
        } while (0)
#endif

#if DEBUG_ENABLE
#    define DEBUG(format, ...)                              \
        do {                                                \
            fmt::print("FUNC: {:<8}, LINE: {:<4}: " format, \
                       __func__,                            \
                       __LINE__,                            \
                       ##__VA_ARGS__);                      \
        } while (0)
#else
#    define DEBUG(format, ...) \
        do {                   \
        } while (0)
#endif

#if ERROR_ENABLE
#    define ERROR(format, ...)                              \
        do {                                                \
            fmt::print("FUNC: {:<8}, LINE: {:<4}: " format, \
                       __func__,                            \
                       __LINE__,                            \
                       ##__VA_ARGS__);                      \
        } while (0)
#else
#    define ERROR(format, ...) \
        do {                   \
        } while (0)
#endif
