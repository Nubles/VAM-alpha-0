#include "Input.h"
#include <GLFW/glfw3.h>

namespace Engine {

std::unordered_map<int, bool> Input::m_keys;
std::unordered_map<int, bool> Input::m_mouseButtons;
float Input::m_mouseX = 0.0f;
float Input::m_mouseY = 0.0f;
float Input::m_lastMouseX = 0.0f;
float Input::m_lastMouseY = 0.0f;
float Input::m_mouseDeltaX = 0.0f;
float Input::m_mouseDeltaY = 0.0f;
bool Input::m_firstMouse = true;

void Input::Init(GLFWwindow* window) {
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    
    // Attempt to get initial cursor pos
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    m_mouseX = static_cast<float>(x);
    m_mouseY = static_cast<float>(y);
    m_lastMouseX = m_mouseX;
    m_lastMouseY = m_mouseY;
}

void Input::Update() {
    // Reset deltas for the frame
    m_mouseDeltaX = m_mouseX - m_lastMouseX;
    m_mouseDeltaY = m_lastMouseY - m_mouseY; // inverted Y coordinate for standard 3D look-up
    m_lastMouseX = m_mouseX;
    m_lastMouseY = m_mouseY;
}

bool Input::IsKeyPressed(int keycode) {
    auto it = m_keys.find(keycode);
    if (it != m_keys.end()) {
        return it->second;
    }
    return false;
}

bool Input::IsMouseButtonPressed(int button) {
    auto it = m_mouseButtons.find(button);
    if (it != m_mouseButtons.end()) {
        return it->second;
    }
    return false;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        m_keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        m_keys[key] = false;
    }
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        m_mouseButtons[button] = true;
    } else if (action == GLFW_RELEASE) {
        m_mouseButtons[button] = false;
    }
}

void Input::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    m_mouseX = static_cast<float>(xpos);
    m_mouseY = static_cast<float>(ypos);
    
    if (m_firstMouse) {
        m_lastMouseX = m_mouseX;
        m_lastMouseY = m_mouseY;
        m_firstMouse = false;
    }
}

} // namespace Engine
