#include <string>
#include <map>
#include <fstream>
#include <cstddef>
#include <logger.hpp>
#include <config.hpp>

ConfigLoader::ConfigLoader(const std::string& configPath, Logger& logger) : mlogger("ConfigLoader", logger), configFile(configPath) { load(); }

bool ConfigLoader::find(const std::string& param) {
    return configEntries.find(param) != configEntries.end();
}

std::string ConfigLoader::get(const std::string& param) {
    return configEntries.at(param);
}

void ConfigLoader::load() {
    mlogger.log("Loading config...");

    if (!configFile.good()) {
        mlogger.error("Loading failed");

        return;
    }

    while (configFile.eof()) {
        std::string line;
        std::getline(configFile, line);

        size_t delimPos = line.find(':');

        configEntries[line.substr(0, delimPos)] = line.substr(delimPos + 1);
    }

    mlogger.log("Loading successful");
}