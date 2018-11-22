#include <jconfig.h>
#include "webservice.h"

jconfig::ConfigFile WebServiceLoader::m_mainConfig;
std::string WebServiceLoader::m_configPath;

std::thread WebServiceLoader::m_wsThread;
bool WebServiceLoader::m_closeWebService = false;

bool WebServiceLoader::findConfig(std::string path, WebServiceArgs* args) {
    initConfig();
    if (!Util::fileExists(path)) {
        std::cout << "[jm] Unable to find config file \"" + path + "\"" << std::endl;
    } else {
        m_configPath = path;
        WebServiceArgs a;
        if (!processConfig(&a)) {
            return false;
        }
        *args = a;
        return true;
    }
    return false;
}

bool WebServiceLoader::processConfig(WebServiceArgs* args) {
    std::string root;
    std::string index;
    std::string missing;
    std::string port;

    std::map<std::string, jconfig::Variable> result;

    jconfig::ProcessError e = m_mainConfig.process(m_configPath, result);
    if (e.error != jconfig::ProcessErrorType::NONE) {
        std::cout << jconfig::getErrorMessage(e);
        return false;
    }

    if (!m_mainConfig.allDefined(result)) {
        std::cout << "[jc] Please make sure all required variables are defined. (" + m_configPath + ")" << std::endl;
        return false;
    }

    root = result["root-dir"].data;
    index = result["index-page"].data;
    missing = result["missing-page"].data;
    port = result["port"].data;
    

    WebServiceArgs a;
    a.m_rootDir = root;
    a.m_indexPage = index;
    a.m_missingPage = missing;
    a.m_port = port;

    *args = a;

    return true;
}

void WebServiceLoader::start(WebServiceArgs args) {
    m_wsThread = std::thread(startWebService, args);
}

void WebServiceLoader::stop() {
    std::cout << "[jm] Stopping WebService..." << std::endl;
    m_closeWebService = true;
    m_wsThread.join();
}

void WebServiceLoader::startWebService(WebServiceArgs args) {
    std::string frootDir = "../assets/" + args.m_rootDir;
    std::string call = "./plugins/webservice --root-dir " + frootDir + " --index-page " + args.m_indexPage + " --missing-page " + args.m_missingPage + " --port " + args.m_port;
    FILE *handle = popen(call.c_str(), "r");

    if (handle == NULL) {
        std::cout << "[jm] Failed to start WebService." << std::endl;
        return;
    }

    std::cout << "[jm] WebService is running." << std::endl;

    char buf[64];
    size_t readn;

    while(!m_closeWebService);

    std::cout << "[jm] WebService was terminated." << std::endl;
    pclose(handle);
}

void WebServiceLoader::initConfig() {
    std::vector<jconfig::Variable> vars;

    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "root-dir"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "index-page"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "missing-page"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "port"));

    m_mainConfig.init(vars);
    m_mainConfig.showWarnings(true);
}
