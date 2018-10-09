#include "mirror.h"

#include "sdl_keyboard_utils.h"

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

    CefRegisterSchemeHandlerFactory("mirror", "res", new ClientSchemeHandlerFactory());

    m_browserClient = new BrowserClient(m_renderHandler);
    m_browser = CefBrowserHost::CreateBrowserSync(windowInfo, m_browserClient.get(), "", browserSettings, nullptr);

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

    m_views[m_svName].makeActive(m_browser);

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
                m_views[m_mvName].makeActive(m_browser);
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

CefBrowserHost::MouseButtonType MainMirror::translateMouseButton(SDL_MouseButtonEvent const &e) {
    CefBrowserHost::MouseButtonType result;
    switch(e.button) {
        case SDL_BUTTON_LEFT:
        case SDL_BUTTON_X1:
            result = MBT_LEFT;
            break;
        case SDL_BUTTON_MIDDLE:
            result = MBT_MIDDLE;
            break;
        case SDL_BUTTON_RIGHT:
        case SDL_BUTTON_X2:
            result = MBT_RIGHT;
            break;
    }
    return result;
}

void MainMirror::processInput() {
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                shutdown();
                break;
            case SDL_KEYDOWN:
                if (evnt.key.keysym.sym == SDLK_BACKSPACE) {
                    CefKeyEvent event;
                    //event.modifiers = getKeyboardModifiers(e.key.keysym.mod);
                    event.windows_key_code = 8;

                    event.type = KEYEVENT_RAWKEYDOWN;
                    m_browser->GetHost()->SendKeyEvent(event);

                    event.type = KEYEVENT_CHAR;
                    m_browser->GetHost()->SendKeyEvent(event);
                    break;
                }
                {
                    CefKeyEvent event;
                    event.modifiers = getKeyboardModifiers(evnt.key.keysym.mod);
                    event.character = getKeyCode(evnt.key.keysym);

                    event.type = KEYEVENT_RAWKEYDOWN;
                    m_browser->GetHost()->SendKeyEvent(event);

                    event.type = KEYEVENT_CHAR;
                    m_browser->GetHost()->SendKeyEvent(event);
                }
                break;
            case SDL_KEYUP:
                {
                    CefKeyEvent event;
                    event.modifiers = getKeyboardModifiers(evnt.key.keysym.mod);
                    event.character = getKeyCode(evnt.key.keysym);

                    event.type = KEYEVENT_KEYUP;
                    m_browser->GetHost()->SendKeyEvent(event);
                }
                break;
            case SDL_WINDOWEVENT:
                switch (evnt.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        m_renderHandler->resize(evnt.window.data1, evnt.window.data2);
                        m_browser->GetHost()->WasResized();
                        break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        m_browser->GetHost()->SetFocus(true);
                        break;

                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        m_browser->GetHost()->SetFocus(false);
                        break;

                    case SDL_WINDOWEVENT_HIDDEN:
                    case SDL_WINDOWEVENT_MINIMIZED:
                       //browser->GetHost()->SetWindowVisibility(false);
                        m_browser->GetHost()->WasHidden(true);
                        break;

                    case SDL_WINDOWEVENT_SHOWN:
                    case SDL_WINDOWEVENT_RESTORED:
                        //browser->GetHost()->SetWindowVisibility(true);
                        m_browser->GetHost()->WasHidden(false);
                        break;

                    case SDL_WINDOWEVENT_CLOSE:
                        evnt.type = SDL_QUIT;
                        SDL_PushEvent(&evnt);
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                {
                    CefMouseEvent event;
                    event.x = evnt.motion.x;
                    event.y = evnt.motion.y;

                    m_browser->GetHost()->SendMouseMoveEvent(event, false);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    CefMouseEvent event;
                    event.x = evnt.button.x;
                    event.y = evnt.button.y;

                    m_browser->GetHost()->SendMouseClickEvent(event, translateMouseButton(evnt.button), true, 1);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    CefMouseEvent event;
                    event.x = evnt.button.x;
                    event.y = evnt.button.y;

                    m_browser->GetHost()->SendMouseClickEvent(event, translateMouseButton(evnt.button), false, 1);
                }
                break;
            case SDL_MOUSEWHEEL:
                {
                    int deltaX = evnt.wheel.x;
                    int deltaY = evnt.wheel.y;

                    if (SDL_MOUSEWHEEL_FLIPPED == evnt.wheel.direction) {
                        deltaY *= -1;
                    } else {
                        deltaX *= -1;
                    }

                    CefMouseEvent event;
                    m_browser->GetHost()->SendMouseWheelEvent(event, deltaX, deltaY);
                }
                break;
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