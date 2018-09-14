#include "view.h"

void View::init(ViewConf config, Awesomium::WebCore* core, Awesomium::WebView* view) {
    m_core = core;
    m_view = view;

    m_name = config.m_name;
    m_type = config.m_type;
    std::string url = "file:///" + Util::workingDir() + "/" + config.m_hPath;
    m_url = Awesomium::WebURL(Awesomium::WSLit(url.c_str()));
    if (m_type == ViewType::SPLASH_VIEW) {
        m_sDuration = config.m_sDuration;
    } else {
        m_sDuration = 0;
    }
}

void View::makeActive() {
    m_view->LoadURL(m_url);

    while(m_view->IsLoading()) {
        m_core->Update();
    }

    Util::sleepms(300);
    m_core->Update();
}

ViewConf ViewLoader::loadView(std::string path) {
    std::cout << "[jm] Attempting to load " + path + "/viewfile..." << std::endl;
    if (!Util::fileExists(path + "/viewfile")) {
        std::cout << "[jm] Unable to open viewfile in \"" + path + "\"" << std::endl;
        return {};
    } else {
        std::string name = "", type = "", html = ""; int splash_duration = 0;
        std::map<std::string, std::string> vars;
        std::ifstream viewfile(path + "/viewfile");
        if (!viewfile.is_open()) {
            std::cout << "[jm] Something went wrong opening viewfile \"" + path + "\"" << std::endl;
            return {};
        } else {
            std::string line;
            while(std::getline(viewfile, line)) {
                std::vector<std::string> p = Util::splitString(Util::removeSpaces(line), ':');
                vars.insert(std::make_pair(p[0], p[1]));
            }
        }
        std::map<std::string, std::string>::iterator it;
        for (it = vars.begin(); it != vars.end(); it++) {
            if (it->first == "view-type") {
                type = it->second;
            } else if (it->first == "view-html") {
                html = it->second;
            } else if (it->first == "splash-duration") {
                try { 
                    splash_duration = std::stoi(it->second);
                } catch (std::invalid_argument) {
                    std::cout << "[jm] Splash duration must be an unsigned integer." << std::endl;
                    std::cout << "[jm] Failed to process viewfile. Halting..." << std::endl;
                    exit(-1);
                }
            } else if (it->first == "view-name") {
                name = it->second;
            } else {
                std::cout << "[jm] Ignoring unknown viewfile argument \"" + it->first + "\"" << std::endl;
            }
        }
        if (name == "" || type == "" || html == "") {
            std::cout << "[jm] Please include a view name, view type, and HTML file in your viewfile." << std::endl;
            std::cout << "[jm] Failed to process viewfile. Halting..." << std::endl;
            exit(-1);
        } else {
            bool fail = false;
            ViewConf v;
            if (type == "view") {
                v.m_type = ViewType::VIEW;
            } else if (type == "splash") {
                v.m_type = ViewType::SPLASH_VIEW;
            } else {
                std::cout << "[jm] Invalid view type specified in viewfile." << std::endl;
                fail = true;
            }
            if (!Util::fileExists(path + "/" + html)) {
                std::cout << "[jm] HTML file \"" + html + "\" does not exist!" << std::endl; 
                fail = true;
            } else {
                v.m_hPath = path + "/" + html;
            }
            if (v.m_type == ViewType::SPLASH_VIEW && splash_duration == 0) {
                std::cout << "[jm] Please specify a splash duration greater than 0" << std::endl;
                fail = true;
            } else {
                v.m_sDuration = splash_duration;
            }
            v.m_name = name;
            if (!fail) {
                std::cout << "[jm] Successfully loaded view \"" + name + "\"!" << std::endl;
                return v;
            } else {
                std::cout << "[jm] Failed to process viewfile. Halting..." << std::endl;
                exit(-1);
            }
        }
    }
}