#pragma once

#include "../../Engine/Math/MathTypes.h"

namespace Game {

class Player {
public:
    Player();
    
    void Update(float dt, bool isMoving, bool isSprinting);
    
    // Getters / Setters
    Engine::Vec3 GetPosition() const { return m_position; }
    void SetPosition(const Engine::Vec3& pos) { m_position = pos; }
    
    float GetHealth() const { return m_health; }
    void SetHealth(float val) { m_health = val; }
    
    float GetStamina() const { return m_stamina; }
    void SetStamina(float val) { m_stamina = val; }
    
    bool IsSprinting() const { return m_isSprinting; }
    float GetSpeed(bool isSprinting) const;

private:
    Engine::Vec3 m_position;
    float m_health;
    float m_stamina;
    float m_maxStamina;
    float m_staminaRegenDelay;
    bool m_isSprinting;
    
    float m_walkSpeed;
    float m_sprintSpeed;
};

} // namespace Game
