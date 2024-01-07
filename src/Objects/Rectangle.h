#pragma once

#include <Objects/Object.h>
#include <Graphic/color.h>

class Rectangle : public Object {

    static uint32_t shaderPipeline;
    static GBuffer buffer;
    static uint16_t freeIndex;

    Color _color;
    float _width;
    float _height;
    uint16_t _index;

public:
    Rectangle(float x, float y, float width, float height, std::shared_ptr<Renderer> renderer, Color color = Color(0xFFFFFFFF));

    void draw(std::shared_ptr<Renderer> renderer) override; 
};