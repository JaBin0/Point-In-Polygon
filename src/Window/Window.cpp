#include "Window.h"
#include <Config/Settings.h>

#include <iostream>

#define GLFW_INCLUDE_NONE0
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

Window* Window::window = nullptr;

Window::Window(std::string_view title, float width, float height) 
    : _window(nullptr, [](GLFWwindow* ptr){if(ptr != nullptr) {glfwDestroyWindow(ptr);}})
    , _mouseAreas {}
    , _activeArea {nullptr}
    , _mouseX {0}
    , _mouseY {0}
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _window.reset(glfwCreateWindow(static_cast<int>(Settings::instance()->screenWidth), static_cast<int>(Settings::instance()->screenHeight), title.data(), nullptr, nullptr));

    if(!_window) {
        std::cout << "ERROR::Window - GLFW window was not created" << std::endl;
        return;
    }
    glfwMakeContextCurrent(_window.get());
    glfwSetCursorPosCallback(_window.get(), Window::mousePosCallback);

    glfwSetMouseButtonCallback(_window.get(), Window::generalMouseButtonCallback);
    // using namespace std::placeholders;
    // callback = std::bind(&PointInPolygon::mouseButtonClbk, this, _1, _2, _3, _4);
    // moveCallback = std::bind(&PointInPolygon::mouseMoveClbk, this, _1, _2, _3);

    // glfwSetMouseButtonCallback(_window.get(), cStyleMouseButtonCallback);
    // glfwSetCursorPosCallback(_window.get(), cStyleMou seMoveCallback);
    // if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    //     std::cout << "ERROR: OpenGL function pointers could not be retrieve by GLAD" << std::endl;
    //     return;        
    // }
}
void Window::generalMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window::instance()->mouseButtonCallback(button, action, mods);
}

void Window::mouseButtonCallback(const int button, const int action, const int mods) {
    if(action == GLFW_RELEASE && _activeArea != nullptr && _activeArea->test(_mouseX, _mouseY)) {
        _activeArea->mousePressed(button, action, mods);
    }
}

void Window::mousePosCallback(GLFWwindow* window, double x, double y) {
    Window::instance()->cursorPosCallback(x, y);
    // std::cout << "<" <<x << ", " << y << ">" << std::endl;
}

void Window::cursorPosCallback(float x, float y) {
    _mouseX = x;
    _mouseY = y;

    if(_activeArea != nullptr) {
        if(_activeArea->active() && _activeArea->test(x,y)) {
            return;
        }
        _activeArea->mouseExit();
        _activeArea = nullptr;
    }

    for(auto mouseArea : _mouseAreas) {
        if(mouseArea->active() && mouseArea->test(x, y)) {
            _activeArea = mouseArea;
            _activeArea->mouseEnter();
            break;
        }
    }
}

GLADloadfunc Window::getProcAddress()
{
    return glfwGetProcAddress;
}


Window::~Window() {
    glfwTerminate();
}

Window* Window::instance() {
    if(window == nullptr) {
        const Settings* settings = Settings::instance();
        window = new Window(settings->title, settings->screenWidth, settings->screenHeight);
    }
    return window;
}

std::tuple<float, float> Window::getCursorPos() {
    double x, y;
    glfwGetCursorPos(_window.get(), &x, &y);
    return {x, y};
}

bool Window::shouldWindowClose() {
    return glfwWindowShouldClose(_window.get());
}

void Window::pollEvents() {
    glfwPollEvents();
}
    
void Window::swapBuffers() {
    glfwSwapBuffers(_window.get());
}

void Window::registerMouseArea(MouseArea* area) {
    _mouseAreas.push_back(area);
}

void Window::unregisterMouseArea(MouseArea* area) {
    if(_activeArea == area) {_activeArea = nullptr;}
    _mouseAreas.erase(std::remove(_mouseAreas.begin(), _mouseAreas.end(), area), _mouseAreas.end());
}
/*
    // using namespace std::placeholders;
    // callback = std::bind(&PointInPolygon::mouseButtonClbk, this, _1, _2, _3, _4);
    // moveCallback = std::bind(&PointInPolygon::mouseMoveClbk, this, _1, _2, _3);

    // glfwSetMouseButtonCallback(_window.get(), cStyleMouseButtonCallback);
    // glfwSetCursorPosCallback(_window.get(), cStyleMouseMoveCallback);
*/

/*

void PointInPolygon::mouseMoveClbk(GLFWwindow* window, double x, double y) {
    // std::cout << "Mouse pos: " << x << ", " << y << std::endl;
}

void PointInPolygon::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawPolygon();

    for(auto i = 0; i < _objects.size(); ++i) {
        _objects.at(i)->draw(_renderer);
    }

    drawUI();

    glPointSize(1.0f);
    glBindVertexArray(0);
    glUseProgram(0);
}


    void mouseButtonClbk(GLFWwindow* window, int button, int action, int mods);

    void mouseMoveClbk(GLFWwindow* window, double x, double y);

    
    // GLFW window handle
    // std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> _window;
*/