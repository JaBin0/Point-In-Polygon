#pragma once
#include <Objects/Object.h>
#include <Graphic/color.h>

class Text : public Object {

    static GBuffer buffer;
    static uint32_t shader;

    // Holds color of the text
    Color _color;

    // Holds size of the text
    float _size;

    // Holds text of the object
    std::string _text;
public:
    Text(float x, float y, std::string text, float size, Color color, std::shared_ptr<Renderer> renderer);
    ~Text();

    void draw(std::shared_ptr<Renderer> renderer) override;

    void text(const std::string text);
};