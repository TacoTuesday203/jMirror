#pragma once

#include <iostream>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#include <SDL.h>

#include "render_handle.h"
#include "browser_client.h"

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
        void init(std::string splashView, std::string mainView, bool lfs, std::vector<std::string> requiredViews);
        //void bindMethods();

        void loop();
        void draw();
        void processInput();

        void shutdown();
    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;

        CefRefPtr<RenderHandler> m_renderHandler = nullptr;
        CefRefPtr<BrowserClient> m_browserClient = nullptr;
        CefRefPtr<CefBrowser> m_browser = nullptr;

        std::map<std::string, View> m_views;

        std::string m_svName;
        std::string m_mvName;

        MirrorState m_state;

        CefBrowserHost::MouseButtonType translateMouseButton(SDL_MouseButtonEvent const &e);
};