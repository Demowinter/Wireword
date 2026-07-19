#pragma once
#include <vector>
#include <map>
#include <functional>
#include <cstdint>
#include <def.hpp>

enum class EventType : uint32_t {
    INPUT   = 0x01U,
    OUTPUT  = 0x04U,
    ERROR   = 0x08U,
    HANGUP  = 0x10U
};

inline EventType operator|(EventType obj1, EventType obj2) {
    return static_cast<EventType>(static_cast<uint32_t>(obj1) | static_cast<uint32_t>(obj2));
}

inline EventType operator&(EventType obj1, EventType obj2) {
    return static_cast<EventType>(static_cast<uint32_t>(obj1) & static_cast<uint32_t>(obj2));
}

using EventCallback = std::function<void(FileDescriptor)>;
using DeferredCallback = std::function<void()>;

struct EventCallbacks {
    EventCallback input = nullptr;
    EventCallback output = nullptr;
    EventCallback error = nullptr;
};


class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void run();
    void stop();

    void addDescriptor(FileDescriptor fd, EventType mask, EventCallbacks cbs = {});
    void removeDescriptor(FileDescriptor fd, bool syscall = true);

    void setEventMask(FileDescriptor fd, EventType mask);
    void setEventCallbacks(FileDescriptor fd, EventCallbacks cbs);

    void addDeferredCallbacks(DeferredCallback cb);

    void setWaitTimeout(int32_t ms);

private:
    void loop();

    bool runflag = false;
    int32_t timeoutMS = 1;

    FileDescriptor epollfd = invalidDesc;

    std::map<FileDescriptor, EventCallbacks> eventCallbacks;
    std::vector<DeferredCallback> deferredCallbacks;
};