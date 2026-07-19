#include <vector>
#include <random>
#include <stdexcept>
#include <cstdlib>
#include <cstddef>
#include <byteo/def.hpp>
#include <byteo/tcp.hpp>
#include <byteo/dns.hpp>
#include <def.hpp>
#include <logger.hpp>
#include <eventloop.hpp>
#include <socket.hpp>

SocketManager::SocketManager(Logger& logger) : mlogger{"SocketManager", logger} {}

SocketManager::~SocketManager() {
    for (auto [handle, ctx] : sockets) {
        byteo::descriptor desc = ctx.desc;

        if (byteo::utils::descriptor_ok(desc)) byteo::close(desc);
    }
}

UHandle SocketManager::bind(const std::string& addr, uint16_t port) {
    mlogger.log("Binding socket...");

    byteo::descriptor desc;

    try {
        desc = byteo::ipv4::tcp::socket();
        byteo::utils::setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, true);
        byteo::bind(desc, byteo::ipv4::dns::resolve(addr, port));
        byteo::listen(desc, 0);

        byteo::utils::setblocking(desc, false);
    }

    catch (std::runtime_error& e) {
        mlogger.error("Socket bind error: ", false);
        mlogger.msg(e.what(), true);

        std::exit(-1);
    }

    FileDescriptor fd = byteo::utils::realfd(desc);
    UHandle handle = hreg.allocate();

    EventCallbacks cbs;
    cbs.input = [this](FileDescriptor fd) { inputHelper(fdmap.at(fd)); };

    sockets[handle.handle] = {desc};
    fdmap[fd] = handle;

    eventloop.addDescriptor(fd, EventType::INPUT, cbs);

    return handle;
}

std::vector<UHandle> SocketManager::accept(UHandle handle) {
    if (!hreg.check(handle)) return {};

    SocketContext& ctx = sockets.at(handle.handle);

    EventCallbacks cbs;
    cbs.input = [this](FileDescriptor fd) { inputHelper(fdmap.at(fd)); };
    cbs.output = [this](FileDescriptor fd) { outputHelper(fdmap.at(fd)); };
    cbs.error = [this](FileDescriptor fd) { errorHelper(fdmap.at(fd)); };

    std::vector<UHandle> accepted;

    while (true) {
        auto result = byteo::accept(ctx.desc);

        if (!result.ok()) break;

        byteo::descriptor newdesc = result.value();
        byteo::utils::setblocking(newdesc, false);

        FileDescriptor fd = byteo::utils::realfd(newdesc);
        UHandle newhandle = hreg.allocate();

        sockets[newhandle.handle] = {newdesc};
        fdmap[fd] = newhandle;

        eventloop.addDescriptor(fd, EventType::INPUT | EventType::OUTPUT, cbs);

        accepted.push_back(newhandle);
    }

    return accepted;
}
    
std::vector<std::byte> SocketManager::read(UHandle handle, int64_t size) {
    if (!hreg.check(handle)) return {};

    SocketContext& ctx = sockets.at(handle.handle);

    return ctx.rbuffer.read(size);
}

int64_t SocketManager::write(UHandle handle, const std::vector<std::byte>& buf) {
    if (!hreg.check(handle)) return {};

    SocketContext& ctx = sockets.at(handle.handle);

    return ctx.wbuffer.write(buf);
}

void SocketManager::close(UHandle handle) {
    if (!hreg.check(handle)) return;

    SocketContext& ctx = sockets.at(handle.handle);
    FileDescriptor fd = invalidDesc;

    if (byteo::utils::descriptor_ok(ctx.desc)) {
        fd = byteo::utils::realfd(ctx.desc);

        byteo::close(ctx.desc);
    }

    eventloop.removeDescriptor(fd, false);
    fdmap.erase(fd);

    sockets.erase(handle.handle);
    hreg.free(handle);
}

void SocketManager::inputHelper(UHandle handle) {

}

void SocketManager::outputHelper(UHandle handle) {

}

void SocketManager::errorHelper(UHandle handle) {

}
