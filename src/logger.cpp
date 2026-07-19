#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <logger.hpp>

void ConsoleLogger::msg(const std::string& message, bool newline) {
    std::cout << message;

    if (newline) std::cout << std::endl;
}

void ConsoleLogger::log(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    std::cout << "[LOG][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) std::cout << std::endl;
}

void ConsoleLogger::warn(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    std::cout << "[WARN][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) std::cout << std::endl;
}

void ConsoleLogger::error(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    std::cerr << "[ERROR][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) std::cout << std::endl;
}

FileLogger::FileLogger(const std::string& logPath) : logFile(logPath, std::ios::app) {}

void FileLogger::msg(const std::string& message, bool newline) {
    logFile << message;

    if (newline) logFile << std::endl;
}

void FileLogger::log(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    logFile << "[LOG][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) logFile << std::endl;
}

void FileLogger::warn(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    logFile << "[WARN][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) logFile << std::endl;
}

void FileLogger::error(const std::string& from, const std::string& message, bool newline) {
    time_t t = time(nullptr);

    logFile << "[ERROR][" << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S") << "][" << from << "] " << message;

    if (newline) logFile << std::endl;
}


ModuleLogger::ModuleLogger(const std::string& moduleName, Logger& logger) : logger(logger), moduleName(moduleName) {}

void ModuleLogger::msg(const std::string& message, bool newline) {
    logger.msg(message, newline);
}

void ModuleLogger::log(const std::string& message, bool newline) {
    logger.log(moduleName, message, newline);
}

void ModuleLogger::warn(const std::string& message, bool newline) {
    logger.warn(moduleName, message, newline);
}

void ModuleLogger::error(const std::string& message, bool newline) {
    logger.error(moduleName, message, newline);
}
