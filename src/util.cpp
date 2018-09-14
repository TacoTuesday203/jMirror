#include "util.h"

namespace Util {
    std::string removeSpaces(std::string str) {
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
        return str;
    }
    std::vector<std::string> splitString(std::string str, char delim) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while(std::getline(tokenStream, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }
    void sleepms(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
    bool fileExists(std::string path) {
        std::ifstream f(path);
        return f.good();
    }
    std::string workingDir() {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        return std::string(cwd);
    }
}