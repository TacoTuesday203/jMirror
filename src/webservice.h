#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

struct WebServiceArgs {
    std::string m_rootDir;
    std::string m_indexPage;
    std::string m_missingPage;
    std::string m_port;
};

class WebServiceLoader {
    public:
        static bool findConfig(std::string path, WebServiceArgs* args);
        static bool processConfig(WebServiceArgs* args);

        static void start(WebServiceArgs args);
        static void stop();
    private:
        static void initConfig();

        //static void startWebService(WebServiceArgs args);

        /*static std::thread m_wsThread;
        static bool m_closeWebService;*/
        
        static FILE* m_handle;

        static jconfig::ConfigFile m_mainConfig;
        static std::string m_configPath;
};