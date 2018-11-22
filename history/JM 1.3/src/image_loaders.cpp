#include "image_loaders.h"

namespace ImageLoader {
    bool loadImage(std::string path, std::string* data) {
        std::vector<unsigned char> buffer;

        std::ifstream file(path, std::ios::binary);
        if (file.fail()) {
            return false;
        }

        file.seekg(0, std::ios::end);
        std::size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        size -= file.tellg();

        buffer.resize(size);
        file.read((char*)&buffer[0], size);

        file.close();

        std::string d;

        for (int i = 0; i < buffer.size(); i++) {
            d += buffer[i];
        }
        *data = d;

        return true;
    }
}