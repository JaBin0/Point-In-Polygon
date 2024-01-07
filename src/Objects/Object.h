#pragma once
#include <memory>

#include <Graphic/Renderer.h>

class Object {
protected:
    float _x;
    float _y;
    // X and Y value converted to the new 0,0 as top left
    float absX;
    float absY;
public: 
    // Constructors && Destructor
    Object(float x, float y, std::shared_ptr<Renderer> renderer);
    virtual ~Object();

    // Draw object via given renderer interface
    virtual void draw(std::shared_ptr<Renderer> renderer) = 0; 
};