#include "Button.h"
#include <iostream>

uint32_t Button::shaderPipeline = Renderer::INVALID_SHADER_PIPELINE;

Button::Button(float x, float y, float w, float h, std::string_view imageOff, std::string_view imageOn, int button, std::shared_ptr<Renderer> renderer) 
    : MouseArea(x, y, w, h, renderer)
    , _imageOn {0}
    , _imageOff {0}
    , _buffer {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE}
    , _state {false}
    , _button {button}
{
    if(shaderPipeline == Renderer::INVALID_SHADER_PIPELINE) {
        shaderPipeline = renderer->createShaderProgram("Assets/Shaders/image.vert", "Assets/Shaders/image.frag");
    }
    _imageOn = renderer->createTexture(imageOn);
    _imageOff = renderer->createTexture(imageOff);

    std::vector<float> data {
        absX,          absY,           0.0f, 1.0f,
        absX + _width, absY,           1.0f, 1.0f,
        absX + _width, absY - _height, 1.0f, 0.0f,
        absX,          absY,           0.0f, 1.0f,
        absX + _width, absY - _height, 1.0f, 0.0f,
        absX,          absY - _height, 0.0f, 0.0f,
    };

    _buffer = renderer->createBuffer(data, {2,2});
}

void Button::draw(std::shared_ptr<Renderer> renderer) {
    renderer->useProgram(shaderPipeline);
    renderer->drawStaticImage(_buffer, 0, _state ? _imageOn : _imageOff);
    renderer->useProgram(0);
}

void Button::mousePressed(int button, int action, int mods) {
    if(button == _button) {
        _state = !_state;
        _pressedCallback(button, _state);
    }
}

void Button::state(const bool state) {
    if(state != _state) {
        _state = state;
        _pressedCallback(-1, _state);
    }
}

bool Button::state() const {
    return _state;
}
