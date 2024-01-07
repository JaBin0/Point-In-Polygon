#pragma once
#include <Objects/Object.h>
#include <Graphic/color.h>
#include <Graphic/Renderer.h>
#include <bitset>

class Point : public Object {

    Color _color;
    uint32_t _index;
    float _radius;

    static GBuffer pointGB;
    static uint32_t pointShader;
    static std::bitset<100> indexes;
public:
    Point(float x, float y, std::shared_ptr<Renderer> renderer, Color color = Color(0xFFFFFFFF));
    ~Point();

    void draw(std::shared_ptr<Renderer> renderer) override; 

    Color color();
    void color(const Color& color);
    void color(const uint32_t color);

    void radius(const float rad);

    float x() const;
    float y() const;

    // Define maximum number of points within Polygon
    static const uint16_t MAX_POINTS = 100;
};