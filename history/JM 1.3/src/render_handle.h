#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#include <SDL.h>

#include <iostream>

class RenderHandler : public CefRenderHandler {
    public:
        RenderHandler(SDL_Renderer* renderer, int w, int h) :
            m_renderer(renderer), m_width(w), m_height(h)
        {
            m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
        }

        ~RenderHandler() noexcept {
            if (m_texture) {
                SDL_DestroyTexture(m_texture);
                m_renderer = nullptr;
            }
        }

        bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
            rect = CefRect(0, 0, m_width, m_height);
            return true;
        }

        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void* buffer, int w, int h) {
            if (m_texture) {
                unsigned char* texture_data = NULL;
                int pitch = 0;

                SDL_LockTexture(m_texture, 0, (void**)&texture_data, &pitch);
                memcpy(texture_data, buffer, w * h * 4);
                SDL_UnlockTexture(m_texture);
            }
        }

        void resize(int w, int h) {
            if (m_texture) {
                SDL_DestroyTexture(m_texture);
            }
            m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_STREAMING, w, h);
            m_width = w;
            m_height = h;
        }

        void render() {
            SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
        }
    private:
        int m_width;
        int m_height;
        SDL_Renderer* m_renderer = nullptr;
        SDL_Texture* m_texture = nullptr;

        IMPLEMENT_REFCOUNTING(RenderHandler);
};