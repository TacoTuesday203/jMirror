#include "mirror.h"

void MainMirror::init(std::string splash, std::string mainView, bool lfs, std::vector<std::string> requiredViews) {
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

    m_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
    m_view = m_core->CreateWebView(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    View sv;
    sv.init(ViewLoader::loadView(splash), m_core, m_view);
    m_svName = sv.getName();

    View mv;
    mv.init(ViewLoader::loadView(mainView), m_core, m_view);
    m_mvName = mv.getName();

    m_views.insert(std::make_pair(m_svName, sv));
    m_views.insert(std::make_pair(m_mvName, mv));

    if (m_views[m_svName].getType() != ViewType::SPLASH_VIEW) {
        std::cout << "[jm] Specified splash view does not have type SPLASH" << std::endl;
        std::cout << "[jm] Failed to start jMirror. Halting..." << std::endl;
        exit(-1);
    }

    for (int i = 0; i < requiredViews.size(); i++) {
        View v;
        v.init(ViewLoader::loadView(requiredViews[i]), m_core, m_view);
        if (v.getName() != "") {
            m_views.insert(std::make_pair(v.getName(), v));
        }
    }

    m_views[m_svName].makeActive();

    m_webSurface = (Awesomium::BitmapSurface*)m_view->surface();

    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0, 0xFF);
    m_webTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, DEFAULT_WIDTH, DEFAULT_HEIGHT);

    std::cout << "[jm] Mirror is ready." << std::endl;

    loop();
}

/*void MainMirror::bindMethods() {
    Awesomium::JSValue result = m_view->CreateGlobalJavascriptObject(Awesomium::WSLit("jmirror"));
    if (result.IsObject()) {
        Awesomium::JSObject& app_object = result.ToObject();

        m_dispatcher.Bind(app_object, Awesomium::WSLit("view"), JSDelegate(this, &MainMirror::onLaunchView));

        m_view->set_js_method_handler(&m_dispatcher);
    }
}*/

/*void MainMirror::onLaunchView(Awesomium::WebView* caller, const Awesomium::JSArray& args) {
    // to be called on jmirror.view("view name") javascript call
    for (int i = 0; i < args.size(); i++) {
        Awesomium::JSValue v = args[i];
        if(v.IsString()) {
            Awesomium::WebString s = v.ToString();
            char* buf;
            s.ToUTF8(buf, s.length());
            std::cout << buf << std::endl;
        }
    }
}*/

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
        m_core->Update();
        m_view->Focus();
        draw();
        processInput();
    }
    while (m_state == MirrorState::SLEEPING) {
        std::cout << "[jm] Error" << std::endl; 
    }
    if (m_state == MirrorState::SHUTDOWN) {
        return;
    }
    loop();
}

void MainMirror::draw() {

    if (m_webSurface != NULL) {
        unsigned char* pixels = nullptr;
        int pitch = 0;
        
        SDL_LockTexture(m_webTexture, nullptr, (void**)&pixels, &pitch);
        m_webSurface->CopyTo(pixels, pitch, 4, true, false);
        SDL_UnlockTexture(m_webTexture);
    }

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_webTexture, NULL, NULL);
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
    Awesomium::WebCore::Shutdown();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    m_state = MirrorState::SHUTDOWN;
    std::cout << "[jm] Goodbye!" << std::endl;
}