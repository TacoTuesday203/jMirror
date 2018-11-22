#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>

#include "util.h"
#include "mirror.h"

class LayoutCheck {
    public:
        static void init(std::vector<std::string> files, std::vector<std::string> folders);
        static bool check();
    private:
        static std::vector<std::string> m_files;
        static std::vector<std::string> m_folders;
        static bool m_init;
    
};

class ConfigLoader {
    public:
        static bool findConfig(std::string path, MirrorArgs* args);
        static bool processConfig(MirrorArgs* args);
    private:
        static void initConfig();

        static jconfig::ConfigFile m_mainConfig;

        static std::string m_configPath;
        //static std::map<std::string, jconfig::Variable> m_cVars;
};