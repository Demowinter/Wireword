#pragma once
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <cstddef>
#include <byteo/def.hpp>
// #include <byteo/ssl.hpp>
#include <def.hpp>
#include <logger.hpp>
#include <ringbuf.hpp>
#include <handlereg.hpp>
#include <eventloop.hpp>

using SocketCallback = std::function<void(UHandle)>;

struct SocketContext {
    byteo::descriptor desc;

    RingBuffer rbuffer{8192};
    RingBuffer wbuffer{8192};

    std::vector<std::byte> overflowBuffer;
};

class SocketManager {
public:
    SocketManager(Logger& logger);
    ~SocketManager();

    UHandle bind(const std::string& addr, uint16_t port);
    std::vector<UHandle> accept(UHandle handle);
    
    std::vector<std::byte> read(UHandle handle, int64_t size);
    int64_t write(UHandle handle, const std::vector<std::byte>& buf);

    void close(UHandle handle);

private:
    void inputHelper(UHandle handle);
    void outputHelper(UHandle handle);
    void errorHelper(UHandle handle);

    ModuleLogger mlogger;
    EventLoop eventloop;
    HandleRegistry hreg;

    std::map<Handle, SocketContext> sockets;
    std::map<FileDescriptor, UHandle> fdmap;
};
