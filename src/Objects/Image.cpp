#include "Image.h"

#include <Graphic/simage.h>
#include <vector>


uint32_t Image::shaderPipeline = Renderer::INVALID_SHADER_PIPELINE;
GBuffer Image::buffer = {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};
uint16_t Image::freeIndex = 0;

Image::Image(const float x, const float y, std::string_view image, std::shared_ptr<Renderer> renderer) 
    : Object(x, y, renderer)
    , _height {0.0f}
    , _width {0.0f}
    , _image {0}
    , _index{0}
{
    if(buffer.vao == Renderer::INVALID_VALUE) {
        buffer = renderer->createBuffer({}, {2,2}, 4 * 6 * 100, false);
        shaderPipeline = renderer->createShaderProgram("Assets/Shaders/image.vert", "Assets/Shaders/image.frag");
    }
    _image = renderer->createTexture(image);
    SImage img(image);
    _height = img.height();
    _width = img.width();

    std::vector<float> data {
        absX,          absY,           0.0f, 1.0f,
        absX + _width, absY,           1.0f, 1.0f,
        absX + _width, absY - _height, 1.0f, 0.0f,
        absX,          absY,           0.0f, 1.0f,
        absX + _width, absY - _height, 1.0f, 0.0f,
        absX,          absY - _height, 0.0f, 0.0f,
    };
    renderer->updateBuffer(buffer, data, data.size(), Image::freeIndex);
    _index = Image::freeIndex++;
}

void Image::draw(std::shared_ptr<Renderer> renderer) {
    renderer->useProgram(shaderPipeline);
    renderer->drawStaticImage(buffer, _index, _image);

    renderer->useProgram(0);
}