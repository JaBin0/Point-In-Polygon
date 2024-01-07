#include "simage.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

SImage::SImage(std::string_view img) 
    : _width {0}
    , _height {0}
    , _nrChannels {0}
    , _data {nullptr}
    , _valid {false}
{
    stbi_set_flip_vertically_on_load(true);
    _data = stbi_load(img.data(), &_width, &_height, &_nrChannels, 0);
    if(nullptr == _data) {
        std::cout << "ERROR::Image - Image <" << img <<"> resource could not been obtain" << std::endl;
        return; 
    }
    _valid = true;
}

SImage::~SImage() {
    STBI_FREE(_data);
}