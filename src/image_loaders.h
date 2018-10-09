#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

// TODO: Implement functions for loading different image types
// List of types to support:
// - PNG!
// - JPEG
// - GIF!
// - BMP?
// Right now, images display improperly

namespace ImageLoader {
    extern bool loadImage(std::string path, std::string* data);
}