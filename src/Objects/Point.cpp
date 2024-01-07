#include "Point.h"
#include <iostream>
#include <Config/Settings.h>

GBuffer Point::pointGB = {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};
uint32_t Point::pointShader = Renderer::INVALID_VALUE;
std::bitset<Point::MAX_POINTS> Point::indexes;
Point::Point(float x, float y, std::shared_ptr<Renderer> renderer, Color color) 
    : Object(x, y, renderer)
    , _color {color}
    , _index{0}
    , _radius {Settings::instance()->radius}
{
    if(Point::pointGB.vao == Renderer::INVALID_VALUE) {
        // 2 float per point * 100 points
        Point::pointGB = renderer->createBuffer({}, {2}, 2 * Point::MAX_POINTS, false);
        Point::pointShader = renderer->createShaderProgram("Assets/Shaders/point.vert", "Assets/Shaders/point.frag");
    }
    for(auto i = 0; i < indexes.size(); i++) {
        if(!indexes.test(i)) {
            _index = i;
            indexes.set(_index);
            break;
        }
    }
    renderer->updateBuffer(pointGB, {absX, absY}, 2, _index);
}

Point::~Point() {
    indexes.reset(_index);
}

void Point::draw(std::shared_ptr<Renderer> renderer) {
    renderer->useProgram(Point::pointShader); 
    renderer->setUniform4f("color", {_color.r(), _color.g(), _color.b(), _color.a()}); 
    renderer->setUniform2f("center", {absX, absY}); 
    renderer->setUniform1f("radius", _radius); 
    renderer->setPointSize(_radius * 2);
    renderer->drawStaticPoint(pointGB, _index); 

    renderer->setPointSize(1.0);
    renderer->useProgram(0);
} 

Color Point::color() {
    return _color;
}

void Point::color(const Color& color) {
    _color = color;
}

void Point::color(const uint32_t color) {
    _color = color;
}

void Point::radius(const float rad) {
    _radius = rad;
}

float Point::x() const {
    return _x;
}

float Point::y() const {
    return _y;
}