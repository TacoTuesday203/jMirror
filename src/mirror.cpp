#include "mirror.h"

void MainMirror::init(std::string splashView, std::string mainView, bool lfs, std::vector<std::string> requiredViews) {        
    m_state = MirrorState::RUNNING;
    if (SDL_Init(SDL_INIT_EVERYTHING < 0)) {
        std::cout << "[jm] Critical: unable to start SDL." << std::endl;
        return;
    }
    m_window = nullptr;
    m_window = SDL_CreateWindow("JMirror", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        std::cout << "[jm] Critical: unable to create SDL window." << std::endl;
        return;
    }

    if (lfs) {
        SDL_SetWindowFullscreen(m_window, 1);
    }

    m_renderer = nullptr;
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        std::cout << "[jm] Critical: could not create render engine." << std::endl;
        return;
    }

    // BEGIN CEF INIT

    //CefRefPtr<RenderHandler> renderHandler = new RenderHandler(m_renderer, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    m_renderHandler = new RenderHandler(m_renderer, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    CefWindowInfo windowInfo;
    CefBrowserSettings browserSettings;

    windowInfo.SetAsWindowless(NULL);

    //CefRefPtr<BrowserClient> browserClient = new BrowserClient(renderHandler);
    m_browserClient = new BrowserClient(m_renderHandler);
    //CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(windowInfo, browserClient.get(), "http://www.google.com", browserSettings, nullptr);
    m_browser = CefBrowserHost::CreateBrowserSync(windowInfo, m_browserClient.get(), "http://www.google.com", browserSettings, nullptr);   

    // END CEF INIT

    for (int i = 0; i < requiredViews.size(); i++) {
        View v;
        v.init(ViewLoader::loadView(requiredViews[i]));
        if (v.getName() != "") {
            m_views.insert(std::make_pair(v.getName(), v));
        } else {
            std::cout << "[jm] Something went wrong loading view \"" + requiredViews[i] + "\"." << std::endl;
            std::cout << "[jm] Failed to start jMirror. Halting..." << std::endl;
            return;
        }
    }

    m_svName = splashView;
    m_mvName = mainView;
    
    if (m_views.find(m_svName) == m_views.end() || m_views.find(m_mvName) == m_views.end()) {
        std::cout << "[jm] Could not find default view/splash view specified in config." << std::endl;
        std::cout << "[jm] Failed to start jMirror. Halting..." << std::endl;
        return;
    }

    if (m_views[m_svName].getType() != ViewType::SPLASH_VIEW) {
        std::cout << "[jm] Specified splash view does not have type SPLASH" << std::endl;
        std::cout << "[jm] Failed to start jMirror. Halting..." << std::endl;
        return;
    }

    m_views[m_svName].makeActive();

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0, 0xFF);

    std::cout << "[jm] Mirror is ready." << std::endl;

    loop();
}

void MainMirror::loop() {
    Timer splash_timer;
    splash_timer.setTime(m_views[m_svName].getsDuration());

    bool splash_running = true;

    while(m_state == MirrorState::RUNNING) {
        if (splash_running) {
            if (!splash_timer.isRunning()) {
                splash_timer.begin();
            }
            splash_timer.update();
            if (splash_timer.isFinished()) {
                splash_running = false;
                m_views[m_mvName].makeActive();
            }
        }
        CefDoMessageLoopWork();

        draw();
        processInput();
    }
    if (m_state == MirrorState::SHUTDOWN) {
        return;
    }
    while (m_state == MirrorState::SLEEPING) {
        std::cout << "[jm] Error" << std::endl; 
    }
}

void MainMirror::draw() {
    SDL_RenderClear(m_renderer);
    m_renderHandler->render();
    SDL_RenderPresent(m_renderer);
}

void MainMirror::processInput() {
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                shutdown();
                break;
            case SDL_KEYDOWN:
                switch(evnt.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        shutdown();
                        break;
                }
        }
    }
}

void MainMirror::shutdown() {
    std::cout << "[jm] Shutting down..." << std::endl;

    m_renderHandler = nullptr;
    m_browserClient = nullptr;
    m_browser = nullptr;

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    CefShutdown();

    std::cout << "[jm] Goodbye!" << std::endl;

    m_state = MirrorState::SHUTDOWN;
}