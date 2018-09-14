#include <iostream>
#include <SDL.h>

//#include "method_dispatcher.h"

#include <WebCore.h>
#include <STLHelpers.h>
#include <BitmapSurface.h>

#include "util.h"
#include "view.h"
#include "timer.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

enum class MirrorState {
    RUNNING, SLEEPING, SHUTDOWN
};

class MainMirror {
    public:
        void init(std::string splash, std::string mainView, bool lfs, std::vector<std::string> requiredViews);
        //void bindMethods();

        void loop();
        void draw();
        void processInput();

        void shutdown();
    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_webTexture;

        Awesomium::WebCore* m_core;
        Awesomium::WebView* m_view;
        Awesomium::BitmapSurface* m_webSurface;

        //MethodDispatcher m_dispatcher;

        std::map<std::string, View> m_views;

        std::string m_svName;
        std::string m_mvName;

        MirrorState m_state;

        //void onLaunchView(Awesomium::WebView* caller, const Awesomium::JSArray& args);
};