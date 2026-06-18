#include "Game.h"
#include "../../Engine/Core/Window.h"
#include "../../Engine/Core/Input.h"
#include "../../Engine/Renderer/Shader.h"
#include "../../Engine/Renderer/Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

namespace Game {

Game::Game()
    : m_window(nullptr), m_running(false),
      m_cubeVAO(0), m_cubeVBO(0),
      m_cubeRotationSpeed(45.0f), m_cubeRotationAngle(0.0f),
      m_showImGuiDemo(false), m_wireframeMode(false),
      m_gridVAO(0), m_gridVBO(0), m_gridVertexCount(0),
      m_targetObjectIndex(-1), m_lastInteractionLog("No interactions yet.") {
    m_cubeColor[0] = 0.2f;
    m_cubeColor[1] = 0.6f;
    m_cubeColor[2] = 0.9f;
}

Game::~Game() {
    Shutdown();
}

bool Game::Init(Engine::Window* window) {
    m_window = window;
    m_running = true;

    // Initialize Database
    ItemDatabase::Get().Initialize();
    CraftingManager::Get().Initialize();
    m_buildSystem.Initialize();

    // Initialize Camera
    m_camera = std::make_unique<Engine::Camera>(glm::vec3(0.0f, 2.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -15.0f);

    // Initialize Shaders
    m_cubeShader = std::make_unique<Engine::Shader>("Assets/Shaders/cube.vert", "Assets/Shaders/cube.frag");
    if (!m_cubeShader->Load()) {
        std::cerr << "[Game::Game] Failed to compile cube shader!" << std::endl;
        return false;
    }

    // Initialize Cube Mesh
    InitCubeMesh();

    // Initialize Grid Mesh
    InitGridMesh();

    // Populate Test Scene Objects
    SceneObject groundGrid;
    groundGrid.name = "Ground Grid";
    groundGrid.type = PrimitiveType::Grid;
    groundGrid.color = Engine::Vec3(0.4f, 0.4f, 0.45f);
    groundGrid.transform.position = Engine::Vec3(0.0f, 0.0f, 0.0f);
    groundGrid.transform.rotation = Engine::Vec3(0.0f, 0.0f, 0.0f);
    groundGrid.transform.scale = Engine::Vec3(1.0f, 1.0f, 1.0f);
    m_sceneObjects.push_back(groundGrid);

    SceneObject monolith;
    monolith.name = "Central Monolith";
    monolith.type = PrimitiveType::Cube;
    monolith.color = Engine::Vec3(0.1f, 0.7f, 0.3f);
    monolith.transform.position = Engine::Vec3(0.0f, 1.0f, 0.0f);
    monolith.transform.rotation = Engine::Vec3(0.0f, 0.0f, 0.0f);
    monolith.transform.scale = Engine::Vec3(1.0f, 2.0f, 1.0f);
    m_sceneObjects.push_back(monolith);

    SceneObject prism;
    prism.name = "Floating Prism";
    prism.type = PrimitiveType::Cube;
    prism.color = Engine::Vec3(0.8f, 0.2f, 0.2f);
    prism.transform.position = Engine::Vec3(3.0f, 3.0f, -2.0f);
    prism.transform.rotation = Engine::Vec3(45.0f, 45.0f, 0.0f);
    prism.transform.scale = Engine::Vec3(0.8f, 0.8f, 0.8f);
    m_sceneObjects.push_back(prism);

    SceneObject marker;
    marker.name = "Stone Marker";
    marker.type = PrimitiveType::Cube;
    marker.color = Engine::Vec3(0.6f, 0.6f, 0.7f);
    marker.transform.position = Engine::Vec3(-4.0f, 0.5f, 3.0f);
    marker.transform.rotation = Engine::Vec3(0.0f, 30.0f, 0.0f);
    marker.transform.scale = Engine::Vec3(0.5f, 1.0f, 0.5f);
    m_sceneObjects.push_back(marker);

    // Milestone 2 Interactable Object
    SceneObject ancientStone;
    ancientStone.name = "Ancient Stone";
    ancientStone.type = PrimitiveType::Cube;
    ancientStone.color = Engine::Vec3(0.3f, 0.3f, 0.4f);
    ancientStone.transform.position = Engine::Vec3(2.0f, 0.6f, 2.0f);
    ancientStone.transform.rotation = Engine::Vec3(0.0f, 45.0f, 0.0f);
    ancientStone.transform.scale = Engine::Vec3(0.8f, 1.2f, 0.8f);
    ancientStone.isInteractable = true;
    ancientStone.interactionRadius = 1.5f;
    ancientStone.interactionMessage = "You touched the cold, weathered surface of the Ancient Stone. The air hums with faint magic.";
    m_sceneObjects.push_back(ancientStone);

    // Drop tables config
    DropTable woodTable;
    woodTable.entries.push_back({"wood", 3, 5});

    DropTable stoneTable;
    stoneTable.entries.push_back({"stone", 2, 4});

    DropTable fiberTable;
    fiberTable.entries.push_back({"fiber", 4, 8});

    // 1. Branch Pile (gives wood, no tool required)
    SceneObject branchPile;
    branchPile.name = "Branch Pile";
    branchPile.type = PrimitiveType::Cube;
    branchPile.color = Engine::Vec3(0.5f, 0.35f, 0.2f); // Brown
    branchPile.transform.position = Engine::Vec3(-2.0f, 0.2f, -3.0f);
    branchPile.transform.rotation = Engine::Vec3(0.0f, 15.0f, 0.0f);
    branchPile.transform.scale = Engine::Vec3(0.6f, 0.3f, 0.6f);
    branchPile.isInteractable = true;
    branchPile.isGatherable = true;
    branchPile.interactionRadius = 1.2f;
    branchPile.gatherable = GatherableNode("Branch Pile", woodTable, 3);
    m_sceneObjects.push_back(branchPile);

    // 2. Loose Stone (gives stone, no tool required)
    SceneObject looseStone;
    looseStone.name = "Loose Stone";
    looseStone.type = PrimitiveType::Cube;
    looseStone.color = Engine::Vec3(0.6f, 0.6f, 0.6f); // Grey
    looseStone.transform.position = Engine::Vec3(3.0f, 0.15f, 4.0f);
    looseStone.transform.rotation = Engine::Vec3(10.0f, 20.0f, 5.0f);
    looseStone.transform.scale = Engine::Vec3(0.4f, 0.3f, 0.4f);
    looseStone.isInteractable = true;
    looseStone.isGatherable = true;
    looseStone.interactionRadius = 1.0f;
    looseStone.gatherable = GatherableNode("Loose Stone", stoneTable, 2);
    m_sceneObjects.push_back(looseStone);

    // 3. Fiber Plant (gives fiber, no tool required)
    SceneObject fiberPlant;
    fiberPlant.name = "Fiber Plant";
    fiberPlant.type = PrimitiveType::Cube;
    fiberPlant.color = Engine::Vec3(0.2f, 0.7f, 0.3f); // Green
    fiberPlant.transform.position = Engine::Vec3(-4.0f, 0.4f, -1.0f);
    fiberPlant.transform.rotation = Engine::Vec3(0.0f, 0.0f, 0.0f);
    fiberPlant.transform.scale = Engine::Vec3(0.3f, 0.8f, 0.3f);
    fiberPlant.isInteractable = true;
    fiberPlant.isGatherable = true;
    fiberPlant.interactionRadius = 1.0f;
    fiberPlant.gatherable = GatherableNode("Fiber Plant", fiberTable, 3);
    m_sceneObjects.push_back(fiberPlant);

    // 4. Cracked Rock (gives stone, requires primitive_tool)
    SceneObject crackedRock;
    crackedRock.name = "Cracked Rock";
    crackedRock.type = PrimitiveType::Cube;
    crackedRock.color = Engine::Vec3(0.4f, 0.4f, 0.45f); // Dark Grey
    crackedRock.transform.position = Engine::Vec3(4.0f, 0.7f, -4.0f);
    crackedRock.transform.rotation = Engine::Vec3(0.0f, 45.0f, 0.0f);
    crackedRock.transform.scale = Engine::Vec3(1.0f, 1.4f, 1.0f);
    crackedRock.isInteractable = true;
    crackedRock.isGatherable = true;
    crackedRock.interactionRadius = 1.8f;
    crackedRock.gatherable = GatherableNode("Cracked Rock", stoneTable, 4, "primitive_tool");
    m_sceneObjects.push_back(crackedRock);

    // 5. Ironbound Sentinel Enemy
    SceneObject sentinel;
    sentinel.name = "Ironbound Sentinel";
    sentinel.type = PrimitiveType::Cube;
    sentinel.color = Engine::Vec3(0.9f, 0.2f, 0.2f); // Red
    sentinel.transform.position = Engine::Vec3(-5.0f, 0.5f, -5.0f);
    sentinel.transform.rotation = Engine::Vec3(0.0f, 0.0f, 0.0f);
    sentinel.transform.scale = Engine::Vec3(1.0f, 1.0f, 1.0f);
    sentinel.isInteractable = true;
    sentinel.interactionRadius = 1.5f;
    sentinel.isEnemy = true;
    sentinel.enemy = Enemy("Ironbound Sentinel", Engine::Vec3(-5.0f, 0.5f, -5.0f));
    m_sceneObjects.push_back(sentinel);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style (Sleek Dark Mode)
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "[Game::Game] Game successfully initialized." << std::endl;
    return true;
}

void Game::InitCubeMesh() {
    // 3D Cube: position (x, y, z) + normal (nx, ny, nz)
    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        // Right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &m_cubeVAO);
    glGenBuffers(1, &m_cubeVBO);

    glBindVertexArray(m_cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::Update(float dt) {
    // Escape key closes the application
    if (Engine::Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
        Stop();
    }

    // Determine movement inputs
    bool isMoving = Engine::Input::IsKeyPressed(GLFW_KEY_W) ||
                    Engine::Input::IsKeyPressed(GLFW_KEY_S) ||
                    Engine::Input::IsKeyPressed(GLFW_KEY_A) ||
                    Engine::Input::IsKeyPressed(GLFW_KEY_D);
    bool isSprinting = Engine::Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ||
                       Engine::Input::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT);

    // Update Player vitals (health, stamina)
    m_player.Update(dt, isMoving, isSprinting);

    // Set player position to camera position
    glm::vec3 camPos = m_camera->GetPosition();
    m_player.SetPosition(Engine::Vec3(camPos.x, camPos.y, camPos.z));

    // Get current speed based on player state
    float speedMultiplier = m_player.GetSpeed(m_player.IsSprinting());
    float cameraDt = dt * (speedMultiplier / 5.0f); // Normalize base speed to match camera base movement

    // Camera keyboard movement controls
    if (Engine::Input::IsKeyPressed(GLFW_KEY_W)) {
        m_camera->ProcessKeyboard(0, cameraDt); // Forward
    }
    if (Engine::Input::IsKeyPressed(GLFW_KEY_S)) {
        m_camera->ProcessKeyboard(1, cameraDt); // Backward
    }
    if (Engine::Input::IsKeyPressed(GLFW_KEY_A)) {
        m_camera->ProcessKeyboard(2, cameraDt); // Left
    }
    if (Engine::Input::IsKeyPressed(GLFW_KEY_D)) {
        m_camera->ProcessKeyboard(3, cameraDt); // Right
    }

    // Toggle mouse capture mode
    static bool cursorCaptured = true;
    if (Engine::Input::IsKeyPressed(GLFW_KEY_LEFT_ALT)) {
        cursorCaptured = false;
        glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else if (Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse) {
        cursorCaptured = true;
        glfwSetInputMode(m_window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    // Mouse camera controls (only rotate camera if cursor is captured/disabled)
    if (cursorCaptured) {
        float dx = Engine::Input::GetMouseDX();
        float dy = Engine::Input::GetMouseDY();
        m_camera->ProcessMouseMovement(dx, dy);
    }

    // Raycast Interaction test
    // Ray starts at camera position in look-at direction
    glm::vec3 rayOrigin = m_camera->GetPosition();
    glm::vec3 rayDir = m_camera->GetFront();

    m_targetObjectIndex = -1;
    float closestDist = 4.0f; // Max interaction/reach distance

    for (int i = 0; i < static_cast<int>(m_sceneObjects.size()); ++i) {
        auto& obj = m_sceneObjects[i];
        if (!obj.isInteractable) continue;

        // Perform simple ray-sphere intersection test as interaction test
        // Object center:
        glm::vec3 center(obj.transform.position.x, obj.transform.position.y, obj.transform.position.z);
        glm::vec3 oc = rayOrigin - center;
        float b = glm::dot(oc, rayDir);
        float c = glm::dot(oc, oc) - (obj.interactionRadius * obj.interactionRadius);
        float discriminant = b * b - c;

        if (discriminant >= 0) {
            float t = -b - sqrt(discriminant);
            if (t > 0 && t < closestDist) {
                closestDist = t;
                m_targetObjectIndex = i;
            }
        }
    }

    // Handle interaction trigger (E key)
    static bool eKeyWasPressed = false;
    bool eKeyPressed = Engine::Input::IsKeyPressed(GLFW_KEY_E);
    if (eKeyPressed && !eKeyWasPressed) {
        if (m_targetObjectIndex != -1) {
            auto& targetObj = m_sceneObjects[m_targetObjectIndex];
            if (targetObj.isGatherable) {
                std::string gatherResult;
                targetObj.gatherable.Gather(m_inventory, gatherResult);
                m_lastInteractionLog = gatherResult;
                std::cout << "[Gathering] " << gatherResult << std::endl;
            } else {
                m_lastInteractionLog = "Interacted with " + targetObj.name + ": " + targetObj.interactionMessage;
                std::cout << "[Interaction] " << m_lastInteractionLog << std::endl;
            }
        }
    }
    eKeyWasPressed = eKeyPressed;

    // Handle Debug Grants (F1 to F5)
    static bool fKeysPressed[5] = {false};
    int grantKeys[5] = {GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5};
    std::string itemIds[5] = {"wood", "stone", "fiber", "raw_food", "primitive_tool"};
    int grantCounts[5] = {10, 10, 15, 5, 1};

    for (int i = 0; i < 5; ++i) {
        bool down = Engine::Input::IsKeyPressed(grantKeys[i]);
        if (down && !fKeysPressed[i]) {
            int leftover = m_inventory.AddItem(itemIds[i], grantCounts[i]);
            int granted = grantCounts[i] - leftover;
            m_lastInteractionLog = "Debug Grant: " + std::to_string(granted) + "x " + itemIds[i];
            if (leftover > 0) {
                m_lastInteractionLog += " (Inventory Full, " + std::to_string(leftover) + " leftover)";
            }
            std::cout << "[Inventory] " << m_lastInteractionLog << std::endl;
        }
        fKeysPressed[i] = down;
    }

    // Hotbar selection keys (1 to 8)
    int hotbarKeys[8] = {GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8};
    for (int i = 0; i < 8; ++i) {
        if (Engine::Input::IsKeyPressed(hotbarKeys[i])) {
            m_hotbar.SelectSlot(i);
        }
    }

    // Scroll wheel hotbar switching
    float scrollY = Engine::Input::GetMouseDY(); // The delta y serves as cursor look, check input system scroll support
    // Standard input might not map scroll, we stick to 1-8 keys and standard updates first.

    // Toggle Build Mode (Key B)
    static bool bKeyWasPressed = false;
    bool bKeyPressed = Engine::Input::IsKeyPressed(GLFW_KEY_B);
    if (bKeyPressed && !bKeyWasPressed) {
        m_buildSystem.ToggleBuildMode();
        m_lastInteractionLog = m_buildSystem.IsBuildMode() ? "Entered Build Mode" : "Exited Build Mode";
        std::cout << "[Build Mode] " << m_lastInteractionLog << std::endl;
    }
    bKeyWasPressed = bKeyPressed;

    // Handle Build Mode input options
    if (m_buildSystem.IsBuildMode()) {
        // Rotate Preview (Arrow keys Left / Right)
        if (Engine::Input::IsKeyPressed(GLFW_KEY_LEFT)) {
            m_buildSystem.RotatePreview(-90.0f * dt);
        }
        if (Engine::Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
            m_buildSystem.RotatePreview(90.0f * dt);
        }

        // Left Click to place piece (only if mouse not hovering UI)
        static bool clickWasPressed = false;
        bool clickPressed = Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        if (clickPressed && !clickWasPressed && !ImGui::GetIO().WantCaptureMouse) {
            glm::vec3 buildTarget = m_buildSystem.GetPlacementTarget(m_camera->GetPosition(), m_camera->GetFront());
            std::string buildMsg;
            if (m_buildSystem.PlacePiece(m_inventory, buildTarget, m_sceneObjects, buildMsg)) {
                m_lastInteractionLog = buildMsg;
                std::cout << "[Build System] " << buildMsg << std::endl;
            } else {
                m_lastInteractionLog = buildMsg;
                std::cout << "[Build Error] " << buildMsg << std::endl;
            }
        }
        clickWasPressed = clickPressed;
    }

    // Apply Combat hits (Left Click in Sandbox) when NOT in build mode
    if (!m_buildSystem.IsBuildMode()) {
        static bool combatClickWasPressed = false;
        bool combatClickPressed = Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        if (combatClickPressed && !combatClickWasPressed && !ImGui::GetIO().WantCaptureMouse) {
            if (m_targetObjectIndex != -1) {
                auto& targetObj = m_sceneObjects[m_targetObjectIndex];
                if (targetObj.isEnemy && !targetObj.enemy.IsDead()) {
                    std::string hitMsg;
                    bool killed = targetObj.enemy.TakeDamage(20.0f, hitMsg);
                    m_lastInteractionLog = hitMsg;
                    std::cout << "[Combat] " << hitMsg << std::endl;
                    if (killed) {
                        std::string lootMsg;
                        targetObj.enemy.DropLoot(m_inventory, lootMsg);
                        m_lastInteractionLog += " | " + lootMsg;
                        std::cout << "[Combat] " << lootMsg << std::endl;
                    }
                }
            }
        }
        combatClickWasPressed = combatClickPressed;
    }

    // Update cube rotation angle for general use
    m_cubeRotationAngle += m_cubeRotationSpeed * dt;
    if (m_cubeRotationAngle > 360.0f) {
        m_cubeRotationAngle -= 360.0f;
    }

    // Dynamic scene objects updates
    for (auto& obj : m_sceneObjects) {
        if (obj.name == "Central Monolith") {
            obj.transform.rotation.y += m_cubeRotationSpeed * dt;
        } else if (obj.name == "Floating Prism") {
            obj.transform.rotation.x += m_cubeRotationSpeed * 0.5f * dt;
            obj.transform.rotation.y += m_cubeRotationSpeed * dt;
            // Float up and down using a sine wave based on runtime
            obj.transform.position.y = 2.5f + static_cast<float>(sin(glfwGetTime())) * 0.7f;
        } else if (obj.isEnemy && !obj.enemy.IsDead()) {
            // Run AI loop
            float playerHealth = m_player.GetHealth();
            std::string aiLog;
            obj.enemy.Update(dt, m_player.GetPosition(), playerHealth, aiLog);
            m_player.SetHealth(playerHealth);
            if (!aiLog.empty()) {
                m_lastInteractionLog = aiLog;
                std::cout << "[AI] " << aiLog << std::endl;
            }
            // Update SceneObject transform coordinates to follow the Enemy position
            Engine::Vec3 enemyPos = obj.enemy.GetPosition();
            obj.transform.position = enemyPos;
        }
    }
}

void Game::Render() {
    // Clear screen buffers
    glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Wireframe mode check
    if (m_wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Bind Shader Program
    m_cubeShader->Use();

    // Compute transformation matrices
    float aspect = static_cast<float>(m_window->GetWidth()) / static_cast<float>(m_window->GetHeight());
    glm::mat4 projection = m_camera->GetProjectionMatrix(aspect);
    glm::mat4 view = m_camera->GetViewMatrix();
    
    // Set view/projection uniforms
    m_cubeShader->SetMat4("projection", projection);
    m_cubeShader->SetMat4("view", view);
    m_cubeShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    m_cubeShader->SetVec3("lightPos", glm::vec3(2.0f, 4.0f, 3.0f));

    // Render all scene objects
    for (const auto& obj : m_sceneObjects) {
        if (obj.isEnemy && obj.enemy.IsDead()) continue;

        glm::mat4 model = obj.transform.GetModelMatrix();
        m_cubeShader->SetMat4("model", model);
        m_cubeShader->SetVec3("objectColor", obj.color);

        if (obj.type == PrimitiveType::Grid) {
            m_cubeShader->SetBool("unlit", true);
            glBindVertexArray(m_gridVAO);
            glDrawArrays(GL_LINES, 0, m_gridVertexCount);
        } else if (obj.type == PrimitiveType::Cube) {
            m_cubeShader->SetBool("unlit", false);
            glBindVertexArray(m_cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    
    // Render Build Placement preview (Ghost Mesh) if build mode is active
    if (m_buildSystem.IsBuildMode()) {
        glm::vec3 buildTarget = m_buildSystem.GetPlacementTarget(m_camera->GetPosition(), m_camera->GetFront());
        const BuildingPiece* selectedPiece = m_buildSystem.GetSelectedPiece();
        if (selectedPiece) {
            // Configure temporary Transform for preview
            Engine::Transform previewTransform;
            previewTransform.position = Engine::Vec3(buildTarget.x, buildTarget.y, buildTarget.z);
            previewTransform.rotation = Engine::Vec3(0.0f, m_buildSystem.GetRotationYaw(), 0.0f);

            if (selectedPiece->type == PieceType::Floor) {
                previewTransform.scale = Engine::Vec3(2.0f, 0.1f, 2.0f);
                previewTransform.position.y = 0.05f;
            } else if (selectedPiece->type == PieceType::Wall) {
                previewTransform.scale = Engine::Vec3(2.0f, 1.5f, 0.15f);
                previewTransform.position.y = 0.75f;
            } else if (selectedPiece->type == PieceType::Roof) {
                previewTransform.scale = Engine::Vec3(2.0f, 0.15f, 2.0f);
                previewTransform.position.y = 1.5f;
            }

            glm::mat4 model = previewTransform.GetModelMatrix();
            m_cubeShader->SetMat4("model", model);
            
            // Set unlit ghost preview colors (semi-transparent blue/white)
            m_cubeShader->SetVec3("objectColor", glm::vec3(0.2f, 0.6f, 1.0f));
            m_cubeShader->SetBool("unlit", true);

            // Render as a wireframe box
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(m_cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            // Reset polygon mode for normal rendering
            glPolygonMode(GL_FRONT_AND_BACK, m_wireframeMode ? GL_LINE : GL_FILL);
        }
    }
    
    glBindVertexArray(0);

    m_cubeShader->Unuse();

    // Disable wireframe for UI rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Render Dear ImGui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderDebugUI(ImGui::GetIO().DeltaTime);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::RenderDebugUI(float dt) {
    // Render Player Vitals HUD (Floating Overlay or top of console)
    ImGui::Begin("Player Vitals & Interaction HUD");
    ImGui::Text("Health:");
    ImGui::SameLine();
    ImGui::ProgressBar(m_player.GetHealth() / 100.0f, ImVec2(200.0f, 0.0f));

    ImGui::Text("Stamina:");
    ImGui::SameLine();
    ImGui::ProgressBar(m_player.GetStamina() / 100.0f, ImVec2(200.0f, 0.0f));

    ImGui::Spacing();
    ImGui::Separator();
    
    // Targeted interactable
    if (m_targetObjectIndex != -1) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Target: %s", m_sceneObjects[m_targetObjectIndex].name.c_str());
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "Press [E] to Interact");
    } else {
        ImGui::Text("Target: None");
    }

    ImGui::Spacing();
    ImGui::TextWrapped("Last Log: %s", m_lastInteractionLog.c_str());
    ImGui::End();

    // Inventory & Hotbar HUD Window
    ImGui::Begin("Player Inventory & Hotbar");
    
    // Render Hotbar Selection
    ImGui::Text("Active Hotbar Slot (Keys 1-8):");
    int activeIdx = m_hotbar.GetActiveIndex();
    for (int i = 0; i < m_hotbar.GetSlotCount(); ++i) {
        int invIdx = m_hotbar.GetInventoryIndex(i);
        const auto& item = m_inventory.GetSlots()[invIdx];
        
        std::string label = "[" + std::to_string(i + 1) + "] ";
        if (item.IsEmpty()) {
            label += "Empty";
        } else {
            label += item.itemId + " (" + std::to_string(item.count) + ")";
        }

        if (i == activeIdx) {
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s <Selected>", label.c_str());
        } else {
            ImGui::Text("%s", label.c_str());
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Full Inventory (24 Slots):");

    // Print inventory slots as a list or simple grid
    if (ImGui::BeginChild("InventorySlotsList", ImVec2(0, 150), true)) {
        const auto& slots = m_inventory.GetSlots();
        for (int i = 0; i < static_cast<int>(slots.size()); ++i) {
            const auto& slot = slots[i];
            std::string slotLabel = "Slot " + std::to_string(i) + ": ";
            if (slot.IsEmpty()) {
                slotLabel += "Empty";
                ImGui::TextDisabled("%s", slotLabel.c_str());
            } else {
                slotLabel += slot.itemId + " (x" + std::to_string(slot.count) + ")";
                ImGui::Text("%s", slotLabel.c_str());
            }
        }
        ImGui::EndChild();
    }

    ImGui::End();

    // Workbench & Crafting HUD Window
    ImGui::Begin("Workbench & Crafting");
    ImGui::Text("Recipes Catalog:");
    ImGui::Separator();

    const auto& recipes = CraftingManager::Get().GetRecipes();
    for (const auto& recipe : recipes) {
        ImGui::Text("%s -> Output: %dx %s", recipe.displayName.c_str(), recipe.outputQuantity, recipe.outputItemId.c_str());
        
        // Render ingredients requirements
        ImGui::Indent(10.0f);
        bool canCraft = true;
        for (const auto& ing : recipe.ingredients) {
            bool owned = m_inventory.Contains(ing.itemId, ing.quantity);
            if (!owned) {
                canCraft = false;
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "- Requires %dx %s (Missing)", ing.quantity, ing.itemId.c_str());
            } else {
                ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "- Requires %dx %s (Owned)", ing.quantity, ing.itemId.c_str());
            }
        }
        ImGui::Unindent(10.0f);

        // Action craft button
        std::string btnLabel = "Craft " + recipe.displayName;
        if (!canCraft) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button(btnLabel.c_str())) {
            std::string craftResult;
            if (CraftingManager::Get().Craft(recipe.id, m_inventory, craftResult)) {
                m_lastInteractionLog = craftResult;
                std::cout << "[Crafting] " << craftResult << std::endl;
            } else {
                m_lastInteractionLog = craftResult;
                std::cout << "[Crafting Error] " << craftResult << std::endl;
            }
        }
        if (!canCraft) {
            ImGui::EndDisabled();
        }
        ImGui::Spacing();
        ImGui::Separator();
    }

    ImGui::End();

    // Building System HUD Window
    ImGui::Begin("Building System");
    bool isBuildMode = m_buildSystem.IsBuildMode();
    if (ImGui::Checkbox("Active Build Mode [B]", &isBuildMode)) {
        m_buildSystem.SetBuildMode(isBuildMode);
    }
    ImGui::Separator();

    if (m_buildSystem.IsBuildMode()) {
        ImGui::Text("Selected Structure Piece:");
        PieceType selectedType = m_buildSystem.GetSelectedPieceType();
        
        const char* pieceNames[] = {"Wood Floor", "Wood Wall", "Fiber Roof"};
        PieceType types[] = {PieceType::Floor, PieceType::Wall, PieceType::Roof};

        for (int i = 0; i < 3; ++i) {
            if (ImGui::RadioButton(pieceNames[i], selectedType == types[i])) {
                m_buildSystem.SetSelectedPieceType(types[i]);
            }
        }

        ImGui::Spacing();
        const BuildingPiece* piece = m_buildSystem.GetSelectedPiece();
        if (piece) {
            ImGui::Text("Materials Cost:");
            if (piece->woodCost > 0) {
                bool owned = m_inventory.Contains("wood", piece->woodCost);
                ImGui::TextColored(owned ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                                   "- Wood: %d required", piece->woodCost);
            }
            if (piece->fiberCost > 0) {
                bool owned = m_inventory.Contains("fiber", piece->fiberCost);
                ImGui::TextColored(owned ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                                   "- Fiber: %d required", piece->fiberCost);
            }
        }

        ImGui::Spacing();
        ImGui::Text("Rotation (Keys LEFT/RIGHT): %.1f deg", m_buildSystem.GetRotationYaw());
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Left Click in Sandbox to place structure");
    } else {
        ImGui::TextDisabled("Build Mode is inactive. Press [B] to toggle.");
    }
    ImGui::End();

    ImGui::Begin("Realmbound Wilds Debug Console");
    
    ImGui::Text("Engine Performance");
    ImGui::Separator();
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    
    ImGui::Spacing();
    ImGui::Text("Camera Status");
    ImGui::Separator();
    glm::vec3 camPos = m_camera->GetPosition();
    ImGui::Text("Position: X:%.2f, Y:%.2f, Z:%.2f", camPos.x, camPos.y, camPos.z);
    if (ImGui::Button("Reset Camera")) {
        m_camera->SetPosition(glm::vec3(0.0f, 2.0f, 6.0f));
    }
    
    ImGui::Spacing();
    ImGui::Text("Environment Controls");
    ImGui::Separator();
    ImGui::Checkbox("Wireframe Mode", &m_wireframeMode);
    ImGui::SliderFloat("Auto-rotation Speed", &m_cubeRotationSpeed, 0.0f, 180.0f);
    
    ImGui::Spacing();
    ImGui::Text("Scene Graph & Inspector");
    ImGui::Separator();
    
    static int selectedObjIdx = 1; // Default select Central Monolith
    if (ImGui::BeginChild("SceneList", ImVec2(0, 100), true)) {
        for (int i = 0; i < static_cast<int>(m_sceneObjects.size()); ++i) {
            std::string label = m_sceneObjects[i].name;
            if (m_sceneObjects[i].type == PrimitiveType::Grid) label += " (Grid)";
            if (m_sceneObjects[i].type == PrimitiveType::Cube) label += " (Cube)";
            
            if (ImGui::Selectable(label.c_str(), selectedObjIdx == i)) {
                selectedObjIdx = i;
            }
        }
        ImGui::EndChild();
    }
    
    if (selectedObjIdx >= 0 && selectedObjIdx < static_cast<int>(m_sceneObjects.size())) {
        auto& selectedObj = m_sceneObjects[selectedObjIdx];
        ImGui::Spacing();
        ImGui::Text("Inspector: %s", selectedObj.name.c_str());
        ImGui::Separator();
        
        // Position controller
        float pos[3] = { selectedObj.transform.position.x, selectedObj.transform.position.y, selectedObj.transform.position.z };
        if (ImGui::DragFloat3("Position", pos, 0.05f)) {
            selectedObj.transform.position = glm::vec3(pos[0], pos[1], pos[2]);
        }
        
        // Rotation controller
        float rot[3] = { selectedObj.transform.rotation.x, selectedObj.transform.rotation.y, selectedObj.transform.rotation.z };
        if (ImGui::DragFloat3("Rotation", rot, 1.0f, 0.0f, 360.0f)) {
            selectedObj.transform.rotation = glm::vec3(rot[0], rot[1], rot[2]);
        }
        
        // Scale controller
        float scl[3] = { selectedObj.transform.scale.x, selectedObj.transform.scale.y, selectedObj.transform.scale.z };
        if (ImGui::DragFloat3("Scale", scl, 0.05f, 0.01f, 10.0f)) {
            selectedObj.transform.scale = glm::vec3(scl[0], scl[1], scl[2]);
        }
        
        // Color controller
        float col[3] = { selectedObj.color.x, selectedObj.color.y, selectedObj.color.z };
        if (ImGui::ColorEdit3("Color", col)) {
            selectedObj.color = glm::vec3(col[0], col[1], col[2]);
        }
    }
    
    ImGui::Spacing();
    ImGui::Checkbox("Show ImGui Demo", &m_showImGuiDemo);
    if (m_showImGuiDemo) {
        ImGui::ShowDemoWindow(&m_showImGuiDemo);
    }
    
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Left-click: Capture Mouse");
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "Left-Alt: Release Mouse");
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "W,A,S,D: Move | Left-Shift: Sprint | E: Interact");
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "F1-F5: Grant Items | 1-8: Select Hotbar Slot | ESC: Quit");

    ImGui::End();
}

void Game::InitGridMesh() {
    std::vector<float> vertices;
    int size = 20;
    int spacing = 2;
    
    for (int z = -size; z <= size; z += spacing) {
        vertices.push_back(static_cast<float>(-size));
        vertices.push_back(0.0f);
        vertices.push_back(static_cast<float>(z));
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
        
        vertices.push_back(static_cast<float>(size));
        vertices.push_back(0.0f);
        vertices.push_back(static_cast<float>(z));
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    }
    
    for (int x = -size; x <= size; x += spacing) {
        vertices.push_back(static_cast<float>(x));
        vertices.push_back(0.0f);
        vertices.push_back(static_cast<float>(-size));
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
        
        vertices.push_back(static_cast<float>(x));
        vertices.push_back(0.0f);
        vertices.push_back(static_cast<float>(size));
        vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    }
    
    m_gridVertexCount = static_cast<int>(vertices.size() / 6);
    
    glGenVertexArrays(1, &m_gridVAO);
    glGenBuffers(1, &m_gridVBO);
    
    glBindVertexArray(m_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Game::Shutdown() {
    if (m_cubeVAO != 0) {
        glDeleteVertexArrays(1, &m_cubeVAO);
        m_cubeVAO = 0;
    }
    if (m_cubeVBO != 0) {
        glDeleteBuffers(1, &m_cubeVBO);
        m_cubeVBO = 0;
    }
    if (m_gridVAO != 0) {
        glDeleteVertexArrays(1, &m_gridVAO);
        m_gridVAO = 0;
    }
    if (m_gridVBO != 0) {
        glDeleteBuffers(1, &m_gridVBO);
        m_gridVBO = 0;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

} // namespace Game
