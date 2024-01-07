#pragma once
#include <string_view>
#include <memory>
#include <functional>
#include <tuple>
#include <vector>

#include <Objects/MouseArea.h>

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE0
#include <glfw/glfw3.h>



class Window {
    // GLFW window handle
    std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> _window;

    std::vector<MouseArea*> _mouseAreas;

    MouseArea* _activeArea;

    // Mouse postion x and y
    float _mouseX;
    float _mouseY;

private: // Methods
    static Window* window;

    Window(std::string_view title, float width, float height);

protected:
    static void generalMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonCallback(const int button, const int action, const int mods);

    static void mousePosCallback(GLFWwindow* window, double x, double y);
    void cursorPosCallback(float x, float y);


public:
    static Window* instance();

    ~Window();

    GLADloadfunc getProcAddress();

    std::tuple<float, float> getCursorPos();

    bool shouldWindowClose();

    void pollEvents();

    void swapBuffers();

    void registerMouseArea(MouseArea* area);
    void unregisterMouseArea(MouseArea* area);
};