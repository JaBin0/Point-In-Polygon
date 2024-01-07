#pragma once
#include <vector>
#include <Objects/MouseArea.h>
#include <Graphic/color.h>
#include <functional>

// struct Point {
//     float x = 0;
//     float y = 0;
// };

class Polygon : public MouseArea {
    // List of points which define polygon
    // std::vector<Point> _points;

    // Graphic buffer which is associated with this polygon
    // GBuffer _buffer;

    bool _edit;

    static uint32_t freeIndex;
    static GBuffer buffer;
    static uint32_t pointShader;
    static uint32_t lineShader;
    static Color color;

    struct Point {
        float x = 0;
        float y = 0;
        // Holds y translate to graphic interface
        float gY = 0;
    };

    std::vector<Point> _points;

    std::weak_ptr<Renderer> _renderer;

    std::function<void(int, bool)> _appCallback;

public:
    // Constructor
    Polygon(float x, float y, float width, float height, std::shared_ptr<Renderer> renderer, std::function<void(int, bool)> callback);

    void draw(std::shared_ptr<Renderer> renderer) override;

    void mousePressed(int button, int action, int mods) override;

    // Setter function which tells polygon if it is currently edited or not
    void edit(const bool state);

    bool isPointInside(float x, float y);

    uint32_t size();

public:
    // Define maximum number of points within Polygon
    static const uint16_t MAX_POINTS = 100;
};