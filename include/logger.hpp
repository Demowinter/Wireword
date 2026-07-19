#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

class Logger {
public:
    virtual ~Logger() = default;

    virtual void msg(const std::string& messgae, bool newline) = 0;
    virtual void log(const std::string& module, const std::string& messgae, bool newline = true) = 0;
    virtual void warn(const std::string& module, const std::string& messgae, bool newline = true) = 0;
    virtual void error(const std::string& module, const std::string& messgae, bool newline = true) = 0;
};

class ConsoleLogger : public Logger {
public:
    void msg(const std::string& message, bool newline) override;
    void log(const std::string& from, const std::string& message, bool newline = true) override;
    void warn(const std::string& from, const std::string& message, bool newline = true) override;
    void error(const std::string& from, const std::string& message, bool newline = true) override;
};

class FileLogger : public Logger {
public:
    FileLogger(const std::string& logPath);

    void msg(const std::string& message, bool newline) override;
    void log(const std::string& from, const std::string& message, bool newline = true) override;
    void warn(const std::string& from, const std::string& message, bool newline = true) override;
    void error(const std::string& from, const std::string& message, bool newline = true) override;

private:
    std::ofstream logFile;
};

class ModuleLogger {
public:
    ModuleLogger(const std::string& moduleName, Logger& logger);

    void msg(const std::string& message, bool newline);
    void log(const std::string& message, bool newline = true);
    void warn(const std::string& message, bool newline = true);
    void error(const std::string& message, bool newline = true);

private:
    Logger& logger;
    std::string moduleName;
};