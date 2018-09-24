#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "util.h"
#include "mirror.h"

class ConfigLoader {
    public:
        static void findConfig(std::string path);
        //static void readConfig();
        static void processConfig();
    private:
        static void initConfig();

        static jconfig::ConfigFile m_mainConfig;

        static std::string m_configPath;
        //static std::map<std::string, jconfig::Variable> m_cVars;
};