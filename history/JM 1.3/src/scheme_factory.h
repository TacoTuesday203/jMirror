#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

#include <iostream>

#include "util.h"
#include "image_loaders.h"


class ClientSchemeHandler : public CefResourceHandler {
    public:
        ClientSchemeHandler() : offset_(0) {}

        bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE {
            CEF_REQUIRE_IO_THREAD();

            std::string url = request->GetURL();

            std::string path = url;
            path.erase(path.begin(), path.begin() + Util::find_nth(path, 3, '/') + 1);
            path.insert(0, Util::workingDir() + "/");

            if (!Util::fileExists(path)) {
                std::cout << "[jm] Unable to open file \"" << path << "\"" << std::endl;
                return false;
            }

            // Ensure files are loaded properly, depending on file extension
            if (url.find(".html") != std::string::npos || url.find(".htm") != std::string::npos) {
                Util::fileToString(path, &data_);
                mime_type_ = "text/html";
            } else if (url.find(".css") != std::string::npos) {
                Util::fileToString(path, &data_);
                mime_type_ = "text/css";
            } else if (url.find(".js") != std::string::npos) {
                Util::fileToString(path, &data_);
                mime_type_ = "text/javascript";  
            } else if (url.find(".png") != std::string::npos) {
                ImageLoader::loadImage(path, &data_);
                mime_type_ = "image/png";
            } else if (url.find(".gif") != std::string::npos)  {
                ImageLoader::loadImage(path, &data_);
                mime_type_ = "image/gif";
            } else if (url.find(".jpg") != std::string::npos || url.find(".jpeg") != std::string::npos) {
                ImageLoader::loadImage(path, &data_);
                mime_type_ = "image/jpeg";  
            } else {
                Util::fileToString(path, &data_);
                std::cout << "[jm] Loaded \"" << path << "\" without setting MIME type." << std::endl;
            }
            //
            
            callback->Continue();
            return true;
        }

        void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE {
            CEF_REQUIRE_IO_THREAD();

            DCHECK(!data_.empty());

            response->SetMimeType(mime_type_);
            response->SetStatus(200);

            response_length = data_.length();
        }

        void Cancel() OVERRIDE {
            CEF_REQUIRE_IO_THREAD();
        }

        bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE {
            CEF_REQUIRE_IO_THREAD();

            bool has_data = false;
            bytes_read = 0;

            if (offset_ < data_.length()) {
                int transfer_size = std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
                memcpy(data_out, data_.c_str() + offset_, transfer_size);
                offset_ += transfer_size;

                bytes_read = transfer_size;
                has_data = true;
            }
            return has_data;
        }
    private:
        std::string data_;
        std::string mime_type_;
        size_t offset_;

        IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
        DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
};

class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
    public:
        ClientSchemeHandlerFactory() {}

        CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE {
            CEF_REQUIRE_IO_THREAD();
            return new ClientSchemeHandler();
        }
    private:
        IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
        DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
};