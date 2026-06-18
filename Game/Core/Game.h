#pragma once

#include <memory>
#include <vector>
#include "SceneObject.h"
#include "Player.h"
#include "ItemDatabase.h"
#include "Inventory.h"
#include "Hotbar.h"

// Forward declarations
namespace Engine {
    class Window;
    class Shader;
    class Camera;
}

namespace Game {

class Game {
public:
    Game();
    ~Game();

    bool Init(Engine::Window* window);
    void Update(float dt);
    void Render();
    void Shutdown();

    bool IsRunning() const { return m_running; }
    void Stop() { m_running = false; }

private:
    void InitCubeMesh();
    void RenderDebugUI(float dt);

    Engine::Window* m_window;
    bool m_running;

    // Rendering resources
    std::unique_ptr<Engine::Shader> m_cubeShader;
    std::unique_ptr<Engine::Camera> m_camera;
    
    unsigned int m_cubeVAO;
    unsigned int m_cubeVBO;
    
    // Debug variables
    float m_cubeRotationSpeed;
    float m_cubeRotationAngle;
    bool m_showImGuiDemo;
    bool m_wireframeMode;
    float m_cubeColor[3];

    // Grid rendering resources
    void InitGridMesh();
    unsigned int m_gridVAO;
    unsigned int m_gridVBO;
    int m_gridVertexCount;

    // Sandbox scene objects
    std::vector<SceneObject> m_sceneObjects;

    // Player and Interaction State
    Player m_player;
    int m_targetObjectIndex;
    std::string m_lastInteractionLog;

    // Inventory & Hotbar
    Inventory m_inventory;
    Hotbar m_hotbar;
};

} // namespace Game
