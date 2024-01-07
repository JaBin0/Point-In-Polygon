#pragma once
#include <Objects/MouseArea.h>

class Button : public MouseArea {
    
    static uint32_t shaderPipeline;
    
    uint32_t _imageOn;
    uint32_t _imageOff;

    GBuffer _buffer;

    bool _state;

    int _button;

public:
    Button(float x, float y, float w, float h, std::string_view imageOff, std::string_view imageOn, int button, std::shared_ptr<Renderer> renderer);

    void draw(std::shared_ptr<Renderer> renderer) override;

    void mousePressed(int button, int action, int mods) override;

    void state(const bool state);
    bool state() const;
};
