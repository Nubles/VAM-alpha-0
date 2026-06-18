#include "Player.h"
#include <algorithm>

namespace Game {

Player::Player()
    : m_position(Engine::Vec3(0.0f)),
      m_health(100.0f),
      m_stamina(100.0f),
      m_maxStamina(100.0f),
      m_staminaRegenDelay(0.0f),
      m_isSprinting(false),
      m_walkSpeed(5.0f),
      m_sprintSpeed(10.0f) {}

void Player::Update(float dt, bool isMoving, bool isSprinting) {
    m_isSprinting = isSprinting && isMoving && m_stamina > 0.0f;

    if (m_isSprinting) {
        // Drain stamina
        m_stamina -= 25.0f * dt;
        if (m_stamina < 0.0f) {
            m_stamina = 0.0f;
            m_isSprinting = false;
        }
        // Set delay before regeneration starts (1.5 seconds)
        m_staminaRegenDelay = 1.5f;
    } else {
        // Countdown delay
        if (m_staminaRegenDelay > 0.0f) {
            m_staminaRegenDelay -= dt;
        }
        
        // Regenerate stamina if delay has expired
        if (m_staminaRegenDelay <= 0.0f && m_stamina < m_maxStamina) {
            m_stamina += 15.0f * dt;
            if (m_stamina > m_maxStamina) {
                m_stamina = m_maxStamina;
            }
        }
    }
}

float Player::GetSpeed(bool isSprinting) const {
    return isSprinting ? m_sprintSpeed : m_walkSpeed;
}

} // namespace Game
