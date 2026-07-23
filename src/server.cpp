#include <string>
#include <csignal>
#include <cstdint>
#include <def.hpp>
#include <logger.hpp>
#include <server.hpp>

Server::Server(Logger& logger) : logger{logger} { instance = this; }

void Server::openClientListener(const std::string& addr, uint16_t port) {
    clientmgr.open(addr, port);
}

void Server::openModuleListener(const std::string& addr, uint16_t port) {
    modulemgr.open(addr, port);
}

void Server::run() {
    mlogger.log("Server running!");

    eventloop.run();
}

void Server::stop() {
    eventloop.stop();

    mlogger.log("Server stopped!");
}

void Server::interruptHandler(int32_t interrupt) {
    if (interrupt == SIGINT) {
        instance->mlogger.log("Received interrupt signal, stopping server...");

        instance->stop();
    }
}