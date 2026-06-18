#pragma once

#include <string>

// Forward declarations
struct GLFWwindow;

namespace Engine {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool Init();
    void Shutdown();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();

    GLFWwindow* GetGLFWWindow() const { return m_window; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
};

} // namespace Engine
