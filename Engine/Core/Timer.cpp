#include "Timer.h"
#include <GLFW/glfw3.h>

namespace Engine {

Timer::Timer()
    : m_deltaTime(0.0f), m_totalTime(0.0f), m_lastTime(0.0f),
      m_fpsTimer(0.0f), m_frameCount(0), m_fps(0) {
    m_lastTime = static_cast<float>(glfwGetTime());
}

void Timer::Tick() {
    float currentTime = static_cast<float>(glfwGetTime());
    m_deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;
    m_totalTime += m_deltaTime;

    // Cap delta time to prevent physics explosions in case of lag/hanging
    if (m_deltaTime > 0.1f) {
        m_deltaTime = 0.1f;
    }

    // Update FPS statistics every second
    m_frameCount++;
    m_fpsTimer += m_deltaTime;
    if (m_fpsTimer >= 1.0f) {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
    }
}

} // namespace Engine
