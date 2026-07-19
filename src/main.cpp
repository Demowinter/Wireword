#include <logger.hpp>
#include <eventloop.hpp>
#include <socket.hpp>

int main() {
    ConsoleLogger cl;
    ModuleLogger ml{"Main", cl};

    // SocketManagers

    ml.log("Hello, World!");
}