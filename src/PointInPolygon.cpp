#include <iostream>
#include <fstream>
#include <numeric>
#include <string_view>
#include <functional>
#include <utility>

#include <Graphic/color.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PointInPolygon.h>
#include <Objects/Polygon.h>
#include <Objects/Rectangle.h>
#include <Objects/Image.h>

#include <Objects/Text.h>

#include <Config/Settings.h>

#include <chrono>
#include <iomanip>

#include <Window/Window.h>
#include <sstream>

static const std::string_view NAME = "Point in Polygon";
static const double VERSION = 1.0;

PointInPolygon::PointInPolygon() 
    : _renderer {nullptr}
    , _objects {}
    , _polygonBtn {nullptr}
    , _addPointBtn {nullptr}
    , _pointsArea {nullptr}
    , _textPoints {nullptr}
    , _textPolygon {nullptr}
    , _editPolygon {false}
    , _addExternalPoint {false}
{
    Settings* settings = Settings::instance();
    _renderer = std::make_shared<Renderer>(Window::instance()->getProcAddress());
   
    // Define application components
    _objects.emplace_back(new Rectangle(0.0f, 0.0f, Settings::instance()->screenWidth, 80.0f, _renderer, Settings::instance()->backgroundColor2));
    _objects.emplace_back(new Rectangle(0.0f, 80.0f, Settings::instance()->screenWidth, 8.0f, _renderer, Settings::instance()->backgroundColor3));
    _objects.emplace_back(new Image(10.0f, 5.0f, "Assets/Images/Logo.png", _renderer));

    using namespace std::placeholders;
    _polygonBtn = std::make_unique<Button>(75.0f, 10.0f, 150.0f, 60.0f, "Assets/Images/PolygonBtnOff.png", "Assets/Images/PolygonBtnOn.png", 0, _renderer);
    _polygonBtn->setPressedFunction(std::bind(&PointInPolygon::editPolygon, this, _1, _2));

    _addPointBtn = std::make_unique<Button>(235.0f, 10.0f, 150.0f, 60.0f, "Assets/Images/AddPointOff.png", "Assets/Images/AddPointOn.png", 0, _renderer);
    _addPointBtn->setPressedFunction(std::bind(&PointInPolygon::addPointBtn, this, _1, _2));

    float posY = 100.0f;
    float margin = Settings::instance()->margin;
    float width = Settings::instance()->screenWidth - (2 * margin);
    float height = Settings::instance()->screenHeight - posY - margin;
    _polygon = std::make_unique<Polygon>(margin, posY, width, height, _renderer, std::bind(&PointInPolygon::editPolygon, this, _1, _2));
    _pointsArea = std::make_unique<MouseArea>(margin, posY, width, height, _renderer);
    _pointsArea->setPressedFunction(std::bind(&PointInPolygon::addPoint, this, _1, _2));
    _pointsArea->active(false);

    _textPolygon = std::make_unique<Text>(790, 55, "", 15.0f, Settings::instance()->TextColor, _renderer);
    _textPoints = std::make_unique<Text>(790, 75, "", 15.0f, Settings::instance()->TextColor, _renderer);
    
}

PointInPolygon::~PointInPolygon() {

}

void PointInPolygon::editPolygon(int button, bool state) {
    if(button == 0 || button == -1) {
        _polygonBtn->state(state);
        _polygon->edit(state);

        if(state == true) {
            _addPointBtn->state(false);
        }
        else {
            for(auto i = 0; i < _points.size(); ++i) {
                float x = _points.at(i)->x();
                float y = _points.at(i)->y();
                Color color = _polygon->isPointInside(x, y) ? Settings::instance()->PointInsideColor : Settings::instance()->PointOutsideColor;
                _points.at(i)->color(color);
            }
        }
    }
}

void PointInPolygon::addPointBtn(int button, bool state) {
    if(state == true) {
        _polygonBtn->state(false);
    }
    _addExternalPoint = state;
    _pointsArea->active(state);
}

void PointInPolygon::addPoint(int button, bool state) {
    if(_addExternalPoint) {
        auto [x, y] = Window::instance()->getCursorPos();
        Color color = _polygon->isPointInside(x, y) ? Settings::instance()->PointInsideColor : Settings::instance()->PointOutsideColor;
        _points.emplace_back(new Point(x, y, _renderer, color));
        _points.back()->radius(6.0f);
    }
}

void PointInPolygon::draw() {
    _renderer->clear();

    for(auto i = 0; i < _objects.size(); ++i) {
        _objects.at(i)->draw(_renderer);
    }

    _polygonBtn->draw(_renderer);
    _addPointBtn->draw(_renderer);

    for(auto i = 0; i < _points.size(); ++i) {
        _points.at(i)->draw(_renderer);
    }

    if(_addExternalPoint) {
        auto [x, y] = Window::instance()->getCursorPos();
        if(_pointsArea->test(x, y)) {
            Point tmp(x, y, _renderer, _polygon->isPointInside(x, y) ? Settings::instance()->PointInsideColor : Settings::instance()->PointOutsideColor);
            tmp.radius(6.0f);
            tmp.draw(_renderer);
        }
    }
    _polygon->draw(_renderer);

    std::stringstream msg;
    msg << "Polygon:            " << std::setw(3) << std::setfill('0') << _polygon->size() << "/" << Polygon::MAX_POINTS;
    _textPolygon->text(msg.str());
    msg.str("");
    msg << "External Points: " << std::setw(3) << std::setfill('0') << _points.size() << "/" << Point::MAX_POINTS;
    _textPoints->text(msg.str());

    _textPolygon->draw(_renderer);
    _textPoints->draw(_renderer);
}

void PointInPolygon::start() {
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    long long avgTime = 0;
    uint64_t frame = 0;
    while(!Window::instance()->shouldWindowClose()) {
        start = std::chrono::steady_clock::now();
        Window::instance()->pollEvents();
        
        draw();
        
        Window::instance()->swapBuffers();
        end = std::chrono::steady_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        avgTime += time.count();
        frame += 1;
        if(!(frame % 30000)) {
            std::cout << "Avg frame time: " << std::setprecision(3) << static_cast<double>(avgTime) / static_cast<double>(frame) << "ms" << std::setprecision(10) << std::endl;
        }
    }

}
// ====================== main ======================
int main() {
    std::cout << "Project: " << NAME << std::endl
              << "   version: " << VERSION << std::endl;
    
    PointInPolygon pip;
    pip.start();
    return 0;

}