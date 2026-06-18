#pragma once

#include <unordered_map>

struct GLFWwindow;

namespace Engine {

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    
    static float GetMouseX() { return m_mouseX; }
    static float GetMouseY() { return m_mouseY; }
    static float GetMouseDX() { return m_mouseDeltaX; }
    static float GetMouseDY() { return m_mouseDeltaY; }

private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

    static std::unordered_map<int, bool> m_keys;
    static std::unordered_map<int, bool> m_mouseButtons;
    
    static float m_mouseX;
    static float m_mouseY;
    static float m_lastMouseX;
    static float m_lastMouseY;
    static float m_mouseDeltaX;
    static float m_mouseDeltaY;
    static bool m_firstMouse;
};

} // namespace Engine
