#include "Object.h"
#include <Config/Settings.h>

Object::Object(float x, float y, std::shared_ptr<Renderer> renderer) 
    : _x {x}
    , _y {y}
    , absX {x}
    , absY {y}
{
    absY = std::abs(y - Settings::instance()->screenHeight); 
}

Object::~Object() {

}