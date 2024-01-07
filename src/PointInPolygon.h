#pragma once
#include <memory>
#include <vector>

#include <Graphic/Renderer.h>
#include <Objects/Object.h>
#include <Objects/Point.h>
#include <Objects/MouseArea.h>
#include <Objects/Button.h>
#include <Objects/Polygon.h>

class Text;

class PointInPolygon {
    // OpenGL manager 
    std::shared_ptr<Renderer> _renderer;

    // Storage for application objects
    std::vector<std::shared_ptr<Object>> _objects; 

    // External points
    std::vector<std::shared_ptr<Point>> _points; 

    // Application buttons
    std::unique_ptr<Button> _polygonBtn;
    std::unique_ptr<Button> _addPointBtn;

    // Polygon 
    std::unique_ptr<Polygon> _polygon;

    // External points area
    std::unique_ptr<MouseArea> _pointsArea;

    std::unique_ptr<Text> _textPoints;
    std::unique_ptr<Text> _textPolygon;

    // Flag which indicate if editing polygon is currently active 
    bool _editPolygon;

    // Flag which indicate if adding external points is active or not 
    bool _addExternalPoint;

public:
    PointInPolygon();
    ~PointInPolygon();

    void draw();
    
    void start();

    void editPolygon(int button, bool state);

    void addPointBtn(int button, bool state);

    void addPoint(int button, bool state);

};