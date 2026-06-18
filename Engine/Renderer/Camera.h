#pragma once

#include <glm/glm.hpp>

namespace Engine {

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = -90.0f, 
           float pitch = 0.0f);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // Getters / Setters
    glm::vec3 GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& pos) { m_position = pos; }
    
    glm::vec3 GetFront() const { return m_front; }
    
    float GetFOV() const { return m_fov; }
    void SetFOV(float fov) { m_fov = fov; }

private:
    void UpdateCameraVectors();

    // Camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    // Euler Angles
    float m_yaw;
    float m_pitch;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_fov;
};

} // namespace Engine
