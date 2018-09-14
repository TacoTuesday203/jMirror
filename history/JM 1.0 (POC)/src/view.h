#include <iostream>
#include <map>

#include <WebCore.h>
#include <STLHelpers.h>

#include "util.h"

enum class ViewType {
    VIEW, SPLASH_VIEW
};

struct ViewConf {
    std::string m_name;
    ViewType m_type;
    unsigned int m_sDuration;
    std::string m_hPath;
};

class View {
    public:
        void init(ViewConf config, Awesomium::WebCore* core, Awesomium::WebView* view);
        void makeActive();

        std::string getName() {return m_name;}
        ViewType getType() {return m_type;}
        unsigned int getsDuration() {return m_sDuration;}
    private:
        std::string m_name = "";

        ViewType m_type;
        unsigned int m_sDuration;

        Awesomium::WebURL m_url;

        Awesomium::WebCore* m_core;
        Awesomium::WebView* m_view;
};

class ViewLoader {
    public:
        static ViewConf loadView(std::string path);
};