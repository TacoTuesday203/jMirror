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
    bool folderExists(std::string path) {
        std::string fp = workingDir() + "/" + path;

        struct stat info;

        if (stat(fp.c_str(), &info) != 0) {
            return false;
        } else if (info.st_mode & S_IFDIR) {
            return true;
        } else {
            return false;
        }
    }
    std::string workingDir() {
        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        return std::string(cwd);
    }
    std::string DumpRequestContents(CefRefPtr<CefRequest> request) {
        std::stringstream ss;

        ss << "URL: " << std::string(request->GetURL());
        ss << "\nMethod: " << std::string(request->GetMethod());

        CefRequest::HeaderMap headerMap;
        request->GetHeaderMap(headerMap);
        if (headerMap.size() > 0) {
            ss << "\nHeaders:";
            CefRequest::HeaderMap::const_iterator it = headerMap.begin();
            for (; it != headerMap.end(); ++it) {
                ss << "\n\t" << std::string((*it).first) << ": " << std::string((*it).second);
            }
        }
        return ss.str();
    }
    size_t find_nth(std::string str, int pos, char what) {
        int o = 0;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == what) {
                o++;
            }
            if (o == pos) {
                return i;
            }
        }
        return 0;
    }
    bool fileToString(std::string path, std::string* data) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while(std::getline(file, line)) {
            data->append(line);
        }

        return true;
    }
}