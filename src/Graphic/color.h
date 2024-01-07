#pragma once
#include <inttypes.h>
#include <array>

class Color {
    uint8_t _color[4];
    uint8_t& _r;
    uint8_t& _g;
    uint8_t& _b;
    uint8_t& _a;
public:
    Color(uint32_t value = 0)
        : Color(static_cast<uint8_t>(value >> 24), static_cast<uint8_t>(value >> 16), static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value)) 
    {};

    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : _color{r, g, b, a}
        , _r {_color[0]}
        , _g {_color[1]}
        , _b {_color[2]}
        , _a {_color[3]} 
    {};

    Color(const Color& color) 
        : _color{color._r, color._g, color._b, color._a}
        , _r {_color[0]}
        , _g {_color[1]}
        , _b {_color[2]}
        , _a {_color[3]} 
    {};

    Color& operator=(const Color& color) {
        _color[0] = color._r;
        _color[1] = color._g;
        _color[2] = color._b;
        _color[3] = color._a;
        return *this;
    }

    float r() const {return static_cast<float>(_r)/255.0f;};
    float g() const {return static_cast<float>(_g)/255.0f;};
    float b() const {return static_cast<float>(_b)/255.0f;};
    float a() const {return static_cast<float>(_a)/255.0f;};

    operator std::array<float, 4>() {
        return {r(), g(), b(), a()};
    }
};