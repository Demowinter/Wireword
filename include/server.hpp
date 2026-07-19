#pragma once
#include <def.hpp>
#include <logger.hpp>
#include <socket.hpp>

class Server {
public:
    Server(Logger& logger);

private:
    ModuleLogger mlogger;

    SocketManager sockmgr;
};