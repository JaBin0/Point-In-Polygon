#include "Rectangle.h"
#include <vector>

uint16_t Rectangle::freeIndex = 0;
uint32_t Rectangle::shaderPipeline = 0;
GBuffer Rectangle::buffer = {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};
Rectangle::Rectangle(float x, float y, float width, float height, std::shared_ptr<Renderer> renderer, Color color) 
    : Object(x, y, renderer)
    , _width {width}
    , _height {height}
    , _color {color}
    , _index {0}
{
    if(buffer.vao == Renderer::INVALID_VALUE) {
        buffer = renderer->createBuffer({}, {2}, 6 * 2 * 100, false);
        shaderPipeline = renderer->createShaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag");
    }

    std::vector<float> data {
        absX, absY, absX + _width, absY, absX + _width, absY - _height,
        absX, absY, absX + _width, absY - _height, absX, absY - _height
    };
    // Single rectangle is describe by 12 values of float type
    renderer->updateBuffer(buffer, data, 12, Rectangle::freeIndex);
    _index = Rectangle::freeIndex++;
}

void Rectangle::draw(std::shared_ptr<Renderer> renderer) {
    renderer->useProgram(shaderPipeline);
    renderer->setUniform4f("color", {_color.r(), _color.g(), _color.b(), _color.a()});
    renderer->drawStaticRectangle(buffer, _index);

    // glBindVertexArray(_backGroundGB.vao);
    // glDrawArrays(GL_TRIANGLES, 0, 6);


    // glDrawArrays(GL_TRIANGLES, 6, 12);

    renderer->useProgram(0);


}