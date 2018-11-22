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
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

namespace Util {
    extern std::string removeSpaces(std::string str);
    extern std::vector<std::string> splitString(std::string str, char delim);
    extern void sleepms(int ms);
    extern bool fileExists(std::string path);
    extern bool folderExists(std::string path);
    extern std::string workingDir();
    extern std::string DumpRequestContents(CefRefPtr<CefRequest> request);
    extern size_t find_nth(std::string str, int pos, char what);
    extern bool fileToString(std::string path, std::string* data);
}