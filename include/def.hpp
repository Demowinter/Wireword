#pragma once
#include <cstdint>


using FileDescriptor = int32_t; // just simple unix fd
using Handle = int32_t; // socket access handle in SocketManager
using Fingerprint = uint64_t;

struct UHandle { // unique handle
    Handle handle;
    Fingerprint fp;
};

inline bool operator==(UHandle obj1, UHandle obj2) {
    return obj1.handle == obj2.handle && obj1.fp == obj2.fp;
}

constexpr FileDescriptor invalidDesc = -1;
constexpr Handle invalidHandle = -1;
constexpr UHandle invalidUHandle = {-1, 0};
