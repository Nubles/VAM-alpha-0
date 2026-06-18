#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_worldUp(up),
      m_yaw(yaw), m_pitch(pitch), m_movementSpeed(5.0f), m_mouseSensitivity(0.1f), m_fov(45.0f) {
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    if (direction == 0) // FORWARD
        m_position += m_front * velocity;
    if (direction == 1) // BACKWARD
        m_position -= m_front * velocity;
    if (direction == 2) // LEFT
        m_position -= m_right * velocity;
    if (direction == 3) // RIGHT
        m_position += m_right * velocity;
    
    // Keep player locked to ground height for basic movement (optional, but good for gameplay)
    // m_position.y = 0.0f; 
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

} // namespace Engine
