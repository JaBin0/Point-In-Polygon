#pragma once
#include <Objects/Object.h>

class Image : public Object {

    static uint32_t shaderPipeline;
    static GBuffer buffer;
    static uint16_t freeIndex;

    float _height;
    float _width;
    uint32_t _image;
    uint16_t _index;
public:
    Image(const float x, const float y, std::string_view image, std::shared_ptr<Renderer> renderer);

    void draw(std::shared_ptr<Renderer> renderer) override;
};