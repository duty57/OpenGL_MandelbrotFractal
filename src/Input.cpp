#include "Input.h"

#include <string>
#include <vec2.hpp>

double Input::zoom = 2.0; // scale factor
int Input::colorMode = 1; // current color mode
glm::dvec2 Input::offset = glm::dvec2(0.0, 0.0);
glm::dvec2 Input::mousePosition = glm::dvec2(0.0, 0.0);
bool Input::autoZoom = false;
double Input::deltaTime = 0; //time spent for rendering frame
double Input::time = glfwGetTime();


void Input::updateZoom(double yOffset) {
    glm::dvec2 mouseWorld = offset + mousePosition * zoom;
    double zoomFactor = 1.0 + deltaTime * std::abs(yOffset);

    if (yOffset > 0) zoom /= zoomFactor;
    else zoom *= zoomFactor;

    offset = mouseWorld - mousePosition * zoom;
}

void Input::mousePosFunction(GLFWwindow *window, double x, double y) {
    //get mouse position
    mousePosition = glm::dvec2(x / (static_cast<float>(WINDOW_WIDTH) / 2) - 1.0,
                               1.0 - y / (static_cast<float>(WINDOW_HEIGHT) / 2));
}

void Input::scrollFunction(GLFWwindow *window, double xOffset, double yOffset) {
    updateZoom(yOffset);
}

void Input::keyFunction(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_ENTER:
            if (action == GLFW_PRESS) autoZoom = !autoZoom;
            break;
        case GLFW_KEY_1:
            if (action == GLFW_PRESS) colorMode = 1;
            break;
        case GLFW_KEY_2:
            if (action == GLFW_PRESS) colorMode = 2;
            break;
        case GLFW_KEY_3:
            if (action == GLFW_PRESS) colorMode = 3;
            break;
        case GLFW_KEY_4:
            if (action == GLFW_PRESS) colorMode = 4;
            break;
        case GLFW_KEY_5:
            if (action == GLFW_PRESS) colorMode = 5;
            break;
        default:
            break;
    }
}

void Input::showFPS(GLFWwindow *window) {
    deltaTime = glfwGetTime() - time;
    time = glfwGetTime();
    std::string title = "mandelbrot[FPS: " + std::to_string(static_cast<int>(round(1 / deltaTime))) + "]";
    glfwSetWindowTitle(window, title.c_str());
}
