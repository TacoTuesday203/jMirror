#include <iostream>
#include <string>
#include <vector>

#include "startup.h"

std::vector<std::string> ValidOptions = {"-c"};
std::string configFile = "";

void processOption(std::string option, std::string parameter);
void start();

int main(int argc, char** argv) {
    bool foundOption = false;
    for (int i = 1; i < argc; i += 2) {
       for (int m = 0; m < ValidOptions.size(); m++) {
           if ((std::string)argv[i] == ValidOptions[m]) {
               foundOption = true;
               processOption(ValidOptions[m], argv[i + 1]);
           }
       }
       if (foundOption == false) {
           std::cout << "[jm] Ignoring invalid option \"" + (std::string)argv[i] + "\"" << std::endl;
       } else {
           foundOption = false;
       }
    }
    start();
    return 0;
}

void processOption(std::string option, std::string parameter) {
    if (option == "-c") {
        std::cout << "[jm] Set config file as \"" + parameter + "\"" << std::endl;
        configFile = parameter;
    }
}

void start() {
    if (configFile == "") {
        std::cout << "[jm] Please specify a config file with \"-c\"" << std::endl;
        return;
    } else {
        ConfigLoader::findConfig(configFile);
    }
}