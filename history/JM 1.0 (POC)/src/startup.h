#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "util.h"
#include "mirror.h"

enum class CVars {
    SPLASH_VIEW, DEFAULT_VIEW
};

class ConfigLoader {
    public:
        static void findConfig(std::string path);
        static void readConfig();
        static void processConfig();
    private:
        static std::string m_configPath;
        static std::map<std::string, std::string> m_cVars;
};