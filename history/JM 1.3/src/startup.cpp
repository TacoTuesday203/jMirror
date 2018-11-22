#include "startup.h"

std::vector<std::string> LayoutCheck::m_files;
std::vector<std::string> LayoutCheck::m_folders;
bool LayoutCheck::m_init = false;

void LayoutCheck::init(std::vector<std::string> files, std::vector<std::string> folders) {
    m_files = files;
    m_folders = folders;
    m_init = true;
}

bool LayoutCheck::check() {
    if (m_files.size() == 0 && m_folders.size() == 0) {
        return true;
    }
    for (int j = 0; j < m_folders.size(); j++) {
        if (!Util::folderExists(m_folders[j])) {
            return false;
        }
    }
    for (int i = 0; i < m_files.size(); i++) {
        if (!Util::fileExists(m_files[i])) {
            return false;
        }
    }
    return true;
}

jconfig::ConfigFile ConfigLoader::m_mainConfig;

std::string ConfigLoader::m_configPath;
//std::map<std::string, jconfig::Variable> ConfigLoader::m_cVars;

bool ConfigLoader::findConfig(std::string path, MirrorArgs* args) {
    initConfig();
    // check for config file
    if (!Util::fileExists(path)) {
        std::cout << "[jm] Unable to find config file \"" + path + "\"" << std::endl;
    } else {
        m_configPath = path;
        MirrorArgs a;
        if (!processConfig(&a)) {
            return false;
        }
        *args = a;
        return true;
    }
    return false;
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

bool ConfigLoader::processConfig(MirrorArgs* args) {
    std::string splashv;
    std::string defaultv;
    bool lfs;
    std::vector<std::string> views;

    std::map<std::string, jconfig::Variable> result;

    jconfig::Error e = m_mainConfig.process(m_configPath, result);
    if (e.error != jconfig::ErrorType::NONE) {
        std::cout << jconfig::getErrorMessage(e) << std::endl;
        return false;
    }

    if (!m_mainConfig.allDefined(result)) {
        std::cout << "[jc] Please make sure all required variables are defined. (" + m_configPath + ")" << std::endl;
        return false;
    }

    splashv = result["splash-view"].data;
    defaultv = result["default-view"].data;
    m_mainConfig.formatList(result["required-views"].data, views);
    lfs = result["fullscreen"].data == "true" ? true : false;

    *args = MirrorArgs(splashv, defaultv, lfs, views);
    return true;
}