#include <Objects/Polygon.h>
#include <Window/Window.h>
#include <Config/Settings.h>

#include <iostream>

uint32_t Polygon::freeIndex = 0;
GBuffer Polygon::buffer = {Renderer::INVALID_VALUE, Renderer::INVALID_VALUE};
uint32_t Polygon::pointShader = Renderer::INVALID_SHADER_PIPELINE;
uint32_t Polygon::lineShader = Renderer::INVALID_SHADER_PIPELINE;
Color Polygon::color(0xe28218ff);

Polygon::Polygon(float x, float y, float width, float height, std::shared_ptr<Renderer> renderer, std::function<void(int, bool)> callback)
    : MouseArea(x, y, width, height, renderer)
    , _edit {false}
    , _points {}
    , _appCallback {callback}
{
    if(pointShader == Renderer::INVALID_SHADER_PIPELINE) {
        pointShader = renderer->createShaderProgram("Assets/Shaders/point.vert", "Assets/Shaders/point.frag");
        lineShader = renderer->createShaderProgram("Assets/Shaders/line.vert", "Assets/Shaders/line.frag");
        buffer = renderer->createBuffer({}, {2}, 2 * 100, false);
    }
    _renderer = renderer;
    active(false);
}

void Polygon::mousePressed(int button, int action, int mods) {
    std::shared_ptr<Renderer> renderer = _renderer.lock();
    if(renderer != nullptr && _edit && button == 0) {
        auto [x, y] = Window::instance()->getCursorPos();
        
        if(_points.size() >=2 && std::pow(x - _points.at(0).x, 2) + std::pow(y - _points.at(0).y, 2) < std::pow(50, 2)) {
            // edit(false);
            _appCallback(0, false);
        }
        else {
            float gY = std::abs(Settings::instance()->screenHeight - y);
            _points.emplace_back(x, y, gY);
            renderer->updateBuffer(buffer, {x, gY}, 2, Polygon::freeIndex++);
        }
    }
    if(_edit && button == 1 && _points.size() > 0) {
        _points.erase(_points.end() - 1, _points.end());
        Polygon::freeIndex--;
    }
}

void Polygon::draw(std::shared_ptr<Renderer> renderer) {
    if(_points.size() >= 2) {
        renderer->useProgram(Polygon::lineShader); 
        renderer->setUniform4f("color", {Polygon::color.r(), Polygon::color.g(), Polygon::color.b(), Polygon::color.a()});
        renderer->setUniform1b("tmpLine", false);
        for(uint32_t i = 0; i < _points.size() - 1; i++) {
            renderer->setUniform2f("startPoint", {_points.at(i).x, _points.at(i).gY});
            renderer->drawStaticLine(buffer, i, 2);
        }
        if(!_edit) {
            renderer->updateBuffer(buffer, {_points.at(0).x, _points.at(0).gY}, 2, Polygon::freeIndex);
            renderer->setUniform2f("startPoint", {_points.at( _points.size() -1).x, _points.at( _points.size() -1).gY});
            renderer->drawStaticLine(buffer, _points.size() - 1, 2);
        }
    }

    renderer->useProgram(Polygon::pointShader); 
    renderer->setUniform4f("color", {Polygon::color.r(), Polygon::color.g(), Polygon::color.b(), Polygon::color.a()}); 
    renderer->setUniform1f("radius", Settings::instance()->radius); 
    renderer->setPointSize(Settings::instance()->radius * 2);
    for(uint32_t i = 0; i < _points.size(); ++i) {
        renderer->setUniform2f("center", {_points.at(i).x, _points.at(i).gY}); 
        renderer->drawStaticPoint(buffer, i); 
    }

    if(_edit && _mouseIn) {
        auto [x, y] = Window::instance()->getCursorPos();
        float gY = std::abs(Settings::instance()->screenHeight - y);
        renderer->updateBuffer(buffer, {x, gY}, 2, Polygon::freeIndex);
        if(_points.size() >= 1) {
            renderer->useProgram(Polygon::lineShader); 
            renderer->setUniform4f("color", {Polygon::color.r() / 2.0f, Polygon::color.g() / 2.0f, Polygon::color.b() / 2.0f, Polygon::color.a() / 2.0f});
            renderer->setUniform2f("startPoint", {_points.back().x,  _points.back().gY});
            renderer->setUniform1b("tmpLine", true);
            renderer->drawStaticLine(buffer, Polygon::freeIndex - 1, 2);
        }
        
        renderer->useProgram(Polygon::pointShader); 
        renderer->setUniform4f("color", {Polygon::color.r() / 2.0f, Polygon::color.g() / 2.0f, Polygon::color.b() / 2.0f, Polygon::color.a() / 2.0f}); 
        renderer->setUniform2f("center", {x, gY}); 
        renderer->setUniform1f("radius", Settings::instance()->radius); 
        renderer->setPointSize(Settings::instance()->radius * 2);
        renderer->drawStaticPoint(buffer, Polygon::freeIndex); 
    }

    renderer->setPointSize(1.0);
    renderer->useProgram(0);
}

void Polygon::edit(const bool state) {
    if(state != _edit) {
        _edit = state;
        if(!_edit &&  _points.size() < 3) {
            _points.erase(_points.begin(), _points.end());
            Polygon::freeIndex = 0;
        }
        active(state);
    }
}

bool Polygon::isPointInside(float x, float y) {
    uint32_t numberOfEgeCrossed = 0;
    // std::cout << "===========" << std::endl;
    if(_points.size() > 2) {
        // Temporarily add first point additionally as a last point to smooth calculation algorithm 
        _points.emplace_back(_points.at(0).x, _points.at(0).y, _points.at(0).gY);
        for(uint32_t i = 1; i < _points.size(); ++i) {
            float x1 = _points.at(i-1).x;
            float y1 = _points.at(i-1).y;

            float x2 = _points.at(i).x;
            float y2 = _points.at(i).y;

            float a = (y2 - y1) / (x2 - x1);
            float b = y1 - (a * x1);

            if(x1 > x2) {std::swap(x1, x2);}
            if(y1 > y2) {std::swap(y1, y2);}

            if(x1 == x2 && x <= x1 && y >= y1 && y <= y2) {
                numberOfEgeCrossed++;
                continue;
            }
            if(y1 == y2 && y == y1 && x >= x1 && x <= x2 ) {
                numberOfEgeCrossed++;
                continue;
            }
            float xp = (y - b) / a;
            if(xp >=x && xp > x1 && xp <= x2) {
                numberOfEgeCrossed++;
                continue;
            }
        }
        _points.erase(_points.end() - 1, _points.end());
    }
    return (numberOfEgeCrossed % 2 == 1);
}

uint32_t Polygon::size() {
    return _points.size();
}