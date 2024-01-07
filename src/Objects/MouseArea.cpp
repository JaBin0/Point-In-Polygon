#include <Objects/MouseArea.h>
#include <Window/Window.h>
#include <iostream>

MouseArea::MouseArea(float x, float y, float w, float h, std::shared_ptr<Renderer> renderer) 
    : Object(x, y, renderer)
    , _width {w}
    , _height {h}
    , _mouseIn{false}
    , _pressedCallback {}
    , _active {true}
{
    Window::instance()->registerMouseArea(this);
}

MouseArea::~MouseArea() {
    Window::instance()->unregisterMouseArea(this);
}

void MouseArea::draw(std::shared_ptr<Renderer> renderer) {

}

void MouseArea::mouseEnter() {
    _mouseIn = true;
}

void MouseArea::mouseExit() {
    _mouseIn = false;
}

void MouseArea::mousePressed(int button, int action, int mods) {
    _pressedCallback(button, action);
}

bool MouseArea::test(float x, float y) const {
    return _active ? (x >= _x && x <= (_x + _width) && y >= _y && y <= (_y + _height)) : false;
} 

void MouseArea::setPressedFunction(std::function<void(int, bool)> callback) {
    _pressedCallback = callback;
}

void MouseArea::active(const bool isActive) {
    _active = isActive;
}

bool MouseArea::active() const {
    return _active;
}