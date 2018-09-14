#include "view.h"

jconfig::ConfigFile ViewLoader::m_viewConfig;
bool ViewLoader::m_init = false;

void View::init(ViewConf config, Awesomium::WebCore* core, Awesomium::WebView* view) {
    if (config.fail) {
        return;
    }

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
    if (!m_init) {
        initConfigFile();
    }

    std::map<std::string, jconfig::Variable> result;

    jconfig::Error e = m_viewConfig.process(path + "/viewfile", result);
    if (e.error != jconfig::ErrorType::NONE) {
        std::cout << jconfig::getErrorMessage(e) << std::endl;
        return ViewConf(true);
    }

    if (result.find("view-name") == result.end() &&
        result.find("view-type") == result.end() &&
        result.find("view-html") == result.end()) {
        std::cout << "[jc] Please make sure all required variables are defined. (" + path + ")" << std::endl;
        return ViewConf(true);
    }

    std::string name, typestr, html, durationstr;
    ViewType type;
    unsigned duration;

    name = result["view-name"].data;
    typestr = result["view-type"].data;
    html = path + "/" + result["view-html"].data;

    if (result.find("splash-duration") == result.end()) {
        durationstr = "0";
    } else {
        durationstr = result["splash-duration"].data;
    }

    if (typestr == "splash") {
        type = ViewType::SPLASH_VIEW;
    } else {
        type = ViewType::VIEW;
    }

    try {
        duration = std::stoi(durationstr);
    } catch(std::invalid_argument& e) {
        std::cout << "[jm] Duration is not an integer." << std::endl;
        return ViewConf(true);
    }

    ViewConf c;
    c.m_name = name;
    c.m_type = type;
    c.m_hPath = html;
    c.m_sDuration = duration;
    return c;
}

void ViewLoader::initConfigFile() {
    std::vector<jconfig::Variable> vars;
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "view-name"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "view-type"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "view-html"));
    vars.push_back(jconfig::Variable(jconfig::VariableType::BASIC, "splash-duration"));

    m_viewConfig.init(vars);
    m_viewConfig.showWarnings(true);

    m_init = true;
    
}