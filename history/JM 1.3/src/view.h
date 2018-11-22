#include <iostream>
#include <map>
#include <cef_client.h>

#include "util.h"

#include <jconfig.h>

enum class ViewType {
    VIEW, SPLASH_VIEW
};

struct ViewConf {
    ViewConf() {}
    ViewConf(bool fail) {fail = this->fail;}

    std::string m_name;
    ViewType m_type;
    unsigned int m_sDuration;
    std::string m_hPath;

    bool fail = false;
};

class View {
    public:
        void init(ViewConf config);
        void makeActive(CefRefPtr<CefBrowser> browser);

        std::string getName() {return m_name;}
        ViewType getType() {return m_type;}
        unsigned int getsDuration() {return m_sDuration;}
    private:
        std::string m_name = "";
        std::string m_url = "";

        ViewType m_type;
        unsigned int m_sDuration;
};

class ViewLoader {
    public:
        static ViewConf loadView(std::string path);
    private:
        static void initConfigFile();

        static jconfig::ConfigFile m_viewConfig;
        static bool m_init;
};