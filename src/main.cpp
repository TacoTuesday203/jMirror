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

std::vector<std::string> ValidOptions = {"-c"};
std::string configFile = "";

void processOption(std::string option, std::string parameter);
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
    std::cout << "[jm] Attempting to load config..." << std::endl;
    ConfigLoader::findConfig("assets/mirror-config");
}

int initCEF(int argc, char** argv) {
    CefMainArgs args(argc, argv);

    int result = CefExecuteProcess(args, nullptr, nullptr);

    if (result >= 0) {
        return result;
    }

    CefSettings settings;

    // CEF settings
    std::ostringstream ss;
    ss << SDL_GetBasePath() << "locales/";

    CefString(&settings.locales_dir_path) = ss.str();
    CefString(&settings.resources_dir_path) = SDL_GetBasePath();
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