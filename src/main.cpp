#include <csignal>
#include <server.hpp>

int main() {
    ConsoleLogger cl;
    Server server{cl};

    ::signal(SIGINT, Server::interruptHandler);

    server.openClientListener("localhost", 8080);
    server.run();
}