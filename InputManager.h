#pragma once
#define GLAD_ONLY_HEADERS
#include "common.h"

class InputManager {
public:
    static void keyboardManager(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void initInputManager(GLFWwindow* window);
    static void mousePosManager(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonManager(GLFWwindow* window, int button, int action, int mods);
    inline static std::map<int, bool> keysState;
    inline static std::map<int, bool> mouseState;
    inline static double posMX, posMY;
};