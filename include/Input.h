#ifndef INPUT_H
#define INPUT_H
#include <vec2.hpp>
#include <GLFW/glfw3.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Input {
public:
    static double zoom; // scale factor
    static int colorMode; // current color mode
    static glm::dvec2 offset;
    static glm::dvec2 mousePosition;
    static bool autoZoom;
    static double deltaTime; //time spent for rendering frame
    static double time;
    
    static void updateZoom(double yOffset);
    static void mousePosFunction(GLFWwindow* window, double x, double y);
    static void scrollFunction(GLFWwindow* window, double xOffset, double yOffset);
    static void keyFunction(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void showFPS(GLFWwindow* window);
};


#endif //INPUT_H
