#include <iostream>
#include <SDL.h>

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

        std::map<std::string, View> m_views;

        std::string m_svName;
        std::string m_mvName;

        MirrorState m_state;
};