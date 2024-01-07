#include "Text.h"

#include <iostream>

uint32_t Text::shader = Renderer::INVALID_SHADER_PIPELINE;
GBuffer Text::buffer = {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};

Text::Text(float x, float y, std::string text, float size, Color color, std::shared_ptr<Renderer> renderer) 
    : Object(x, y, renderer)
    , _color{color}
    , _size{size}
    , _text{text}
{
    if(shader == Renderer::INVALID_SHADER_PIPELINE) {
        buffer = renderer->createBuffer({}, {2,2}, 6 * 4, false);
        shader = renderer->createShaderProgram("Assets/Shaders/text.vert", "Assets/Shaders/text.frag");
    }
}

Text::~Text() {

}

void Text::draw(std::shared_ptr<Renderer> renderer) {
    renderer->useProgram(shader);
    renderer->setUniform4f("color", _color);
    renderer->drawText2D(buffer, _text, absX, absY, _size);
}

void Text::text(const std::string text) {
    _text = text;
}