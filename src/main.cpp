#include <iostream>
#include <string>
#include <vector>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#include "startup.h"

void start();
int initCEF(int argc, char** argv);

int main(int argc, char** argv) {
    if (initCEF(argc, argv) < 0) {
        std::cout << "[jm] Unable to start CEF." << std::endl;
        return -1;
    }

    start();
    return 0;
}

void start() {
    std::vector<std::string> folders;
    folders.push_back("plugins");                         // Plugins folder (additional services)
    folders.push_back("../assets");                       // Assets folder
    folders.push_back("../assets/web-modules");           // WebModules folder

    std::vector<std::string> files;
    files.push_back("plugins/webservice");                // WebService
    files.push_back("../assets/web-config");              // WebService config
    files.push_back("../assets/web-modules/error.html");  // WebService error page
    files.push_back("../assets/web-modules/index.html");  // WebService index page (required)
    files.push_back("../assets/mirror-config");           // Mirror config file

    LayoutCheck::init(files, folders);

    std::cout << "[jm] Ensuring layout..." << std::endl;
    if (!LayoutCheck::check()) {
        std::cout << "[jm] Error ensuring layout. An important file/folder may be missing." << std::endl;
        return;
    }

    std::cout << "[jm] Attempting to load configs..." << std::endl;

    MirrorArgs margs;
    if (!ConfigLoader::findConfig("../assets/mirror-config", &margs)) {
        std::cout << "[jm] Failed while loading mirror config file." << std::endl;
        return;
    }
    
    WebServiceArgs wargs;
    if (!WebServiceLoader::findConfig("../assets/web-config", &wargs)) {
        std::cout << "[jm] Failed while loading web config file." << std::endl;
        std::cout << "[jm] Continuing without starting WebService." << std::endl;
    } else {
        WebServiceLoader::start(wargs);
    }

    MainMirror m;
    m.init(margs);

    WebServiceLoader::stop();

    CefShutdown();

    std::cout << "[jm] Goodbye!" << std::endl;
}

int initCEF(int argc, char** argv) {
    CefMainArgs args(argc, argv);

    int result = CefExecuteProcess(args, nullptr, nullptr);

    if (result >= 0) {
        return result;
    }

    CefSettings settings;

    // CEF settings
    std::ostringstream ss_locales;
    ss_locales << SDL_GetBasePath() << "cef/locales/";

    std::ostringstream ss_resources;
    ss_resources << SDL_GetBasePath() << "cef/";

    CefString(&settings.locales_dir_path) = ss_locales.str();
    CefString(&settings.resources_dir_path) = ss_resources.str();
    settings.log_severity = LOGSEVERITY_DISABLE; // disable console log outputs

    // Initialize CEF
    {
        bool result = CefInitialize(args, settings, nullptr, nullptr);
        if (!result) {
            return -1;
        }
    }

    std::cout << "[jm] Early CEF init complete." << std::endl;

    return 0;
}