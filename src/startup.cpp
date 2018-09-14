#include "startup.h"

jconfig::ConfigFile ConfigLoader::m_mainConfig;

std::string ConfigLoader::m_configPath;
//std::map<std::string, jconfig::Variable> ConfigLoader::m_cVars;

void ConfigLoader::findConfig(std::string path) {
    initConfig();
    // check for config file
    if (!Util::fileExists(path)) {
        std::cout << "[jm] Unable to find config file \"" + path + "\"" << std::endl;
    } else {
        m_configPath = path;
        processConfig();
    }
}

void ConfigLoader::initConfig() {
    std::vector<jconfig::Variable> vars;

    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "splash-view"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "default-view"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::LIST, "required-views"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "fullscreen"));

    m_mainConfig.init(vars);
    m_mainConfig.showWarnings(true);
}

/*void ConfigLoader::readConfig() {
    jconfig::Error e = m_mainConfig.process(m_configPath, m_cVars);
    if (e.error != jconfig::ErrorType::NONE) {
        std::cout << jconfig::getErrorMessage(e) << std::endl;
        return;
    }
    processConfig();
}*/

// tomorrow, the great big rewrite of this mess begins

void ConfigLoader::processConfig() {
    std::string splashv;
    std::string defaultv;
    bool lfs;
    std::vector<std::string> views;

    std::map<std::string, jconfig::Variable> result;

    jconfig::Error e = m_mainConfig.process(m_configPath, result);
    if (e.error != jconfig::ErrorType::NONE) {
        std::cout << jconfig::getErrorMessage(e) << std::endl;
        return;
    }

    if (!m_mainConfig.allDefined(result)) {
        std::cout << "[jc] Please make sure all required variables are defined. (" + m_configPath + ")" << std::endl;
        return;
    }

    splashv = result["splash-view"].data;
    defaultv = result["default-view"].data;
    m_mainConfig.formatList(result["required-views"].data, views);
    lfs = result["fullscreen"].data == "true" ? true : false;

    std::cout << "[jm] Starting mirror process..." << std::endl;

    MainMirror m;
    m.init(splashv, defaultv, lfs, views);
}