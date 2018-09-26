#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#include <SDL.h>

#include <iostream>

class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
    public:
        BrowserClient(CefRefPtr<CefRenderHandler> ptr) :
            m_handler(ptr)
        {}

        virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() {
            return this;
        }

        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() {
            return this;
        }

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
            return m_handler;
        }

        void OnAfterCreated(CefRefPtr<CefBrowser> browser) {
            CEF_REQUIRE_UI_THREAD();

            m_browserID = browser->GetIdentifier();
        }

        bool DoClose(CefRefPtr<CefBrowser> browser) {
            CEF_REQUIRE_UI_THREAD();

            if (browser->GetIdentifier() == m_browserID) {
                m_closing = true;
            }

            return false;
        }

        void OnBeforeClose(CefRefPtr<CefBrowser> browser) {}

        void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
            m_loaded = true;
        }

        bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString& failedUrl, CefString& errorText) {
            std::cout << "ERROR: " << errorText << " (" << failedUrl << ")" << std::endl;
            m_loaded = true;
        }

        void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {}

        void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {}

        bool closeAllowed() const {
            return m_closing;
        }

        bool isLoaded() const {
            return m_loaded;
        }
    private:
        int m_browserID;

        bool m_closing = false;
        bool m_loaded = false;

        CefRefPtr<CefRenderHandler> m_handler;

        IMPLEMENT_REFCOUNTING(BrowserClient);
};