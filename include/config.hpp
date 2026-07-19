#pragma once
#include <string>
#include <map>
#include <fstream>
#include <cstddef>
#include <logger.hpp>

class ConfigLoader {
public:
    ConfigLoader(const std::string& configPath, Logger& logger);

    bool find(const std::string& param);
    std::string get(const std::string& param);

private:
    void load();

    ModuleLogger mlogger;

    std::map<std::string, std::string> configEntries;
    std::ifstream configFile;
};