#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <fstream>
#include <unistd.h>
#include <limits.h>

namespace Util {
    extern std::string removeSpaces(std::string str);
    extern std::vector<std::string> splitString(std::string str, char delim);
    extern void sleepms(int ms);
    extern bool fileExists(std::string path);
    extern std::string workingDir();
}