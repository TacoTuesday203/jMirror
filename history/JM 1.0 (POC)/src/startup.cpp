#include "startup.h"

std::string ConfigLoader::m_configPath;
std::map<std::string, std::string> ConfigLoader::m_cVars;

void ConfigLoader::findConfig(std::string path) {
    // check for config file
    if (!Util::fileExists(path)) {
        std::cout << "[jm] Unable to find config file \"" + path + "\"" << std::endl;
    } else {
        m_configPath = path;
        readConfig();
    }
}

void ConfigLoader::readConfig() {
    std::ifstream config(m_configPath);
    if (!config.is_open()) {
        std::cout << "[jm] Something went wrong opening your config file." << std::endl;
        return;
    } else {
        std::string line;
        while(std::getline(config, line)) {
            std::vector<std::string> p = Util::splitString(Util::removeSpaces(line), ':');
            m_cVars.insert(std::make_pair(p[0], p[1]));
        }
    }
    processConfig();
}

// tomorrow, the great big rewrite of this mess begins

void ConfigLoader::processConfig() {
   std::string splashv = "", defaultv = ""; bool lfs = false; std::vector<std::string> views;
    std::map<std::string, std::string>::iterator it;
    for (it = m_cVars.begin(); it != m_cVars.end(); it++) {
        if (it->first == "splash-view") {
            splashv = it->second;
        } else if (it->first == "default-view") {
            defaultv = it->second;
        } else if (it->first == "fullscreen") {
            if (it->second == "true") {
                lfs = true;
            } else if (it->second != "false") {
                std::cout << "[jm] Please specify either true or false for fullscreen." << std::endl;
            }
        } else if (it->first == "required-views") {
            views = Util::splitString(it->second, ',');
        } else {
            std::cout << "[jm] Ignoring unknown configuration argument \"" + it->first + "\"" << std::endl;
        }
    }
    if (splashv == "" || defaultv == "") {
        std::cout << "[jm] Please define a splash view and default view in your config file." << std::endl;
        return;
    }
    std::cout << "[jm] Starting mirror process..." << std::endl;
    MainMirror m;
    m.init(splashv, defaultv, lfs, views);
}