#pragma once

namespace Engine {

class Timer {
public:
    Timer();
    
    void Tick();
    
    float GetDeltaTime() const { return m_deltaTime; }
    float GetTotalTime() const { return m_totalTime; }
    unsigned int GetFPS() const { return m_fps; }

private:
    float m_deltaTime;
    float m_totalTime;
    float m_lastTime;
    
    // FPS counting
    float m_fpsTimer;
    unsigned int m_frameCount;
    unsigned int m_fps;
};

} // namespace Engine
