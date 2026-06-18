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
            m_lastInteractionLog = "Interacted with " + targetObj.name + ": " + targetObj.interactionMessage;
            std::cout << "[Interaction] " << m_lastInteractionLog << std::endl;
        }
    }
    eKeyWasPressed = eKeyPressed;

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
    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "W,A,S,D: Move | Left-Shift: Sprint | E: Interact | ESC: Quit");

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
