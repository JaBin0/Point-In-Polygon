#pragma once
#include <functional>

#include <Objects/Object.h>

class MouseArea : public Object{
protected:
    // Width of the mouse area
    float _width;

    // Height of the mouse area
    float _height;

    // Flag which indicate if the mouse is within mouse area
    bool _mouseIn;

    std::function<void (int, bool)> _pressedCallback;

    bool _active;

public:
    MouseArea(float x, float y, float w, float h, std::shared_ptr<Renderer> renderer); 
    ~MouseArea();

    void draw(std::shared_ptr<Renderer> renderer) override;

    virtual void mouseEnter();

    virtual void mouseExit();

    virtual void mousePressed(int button, int action, int mods);

    virtual bool test(float x, float y) const;

    void setPressedFunction(std::function<void(int, bool)> callback);

    void active(const bool isActive);

    bool active() const;
};