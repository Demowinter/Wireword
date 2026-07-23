#pragma once
#include <string>
#include <cstdint>
#include <def.hpp>
#include <logger.hpp>
#include <eventloop.hpp>
#include <handlereg.hpp>
#include <socket.hpp>

class Server {
public:
    Server(Logger& logger);

    void openClientListener(const std::string& addr, uint16_t port);
    void openModuleListener(const std::string& addr, uint16_t port);

    void run();
    void stop();

    static void interruptHandler(int32_t interrupt);

private:
    Logger& logger;
    ModuleLogger mlogger{"Server", logger};

    EventLoop eventloop;
    HandleRegistry hreg;

    SocketManager clientmgr{logger, eventloop, hreg};
    SocketManager modulemgr{logger, eventloop, hreg};

    static inline Server* instance = nullptr;
};