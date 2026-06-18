#pragma once

#include <string>
#include <vector>
#include "../../Engine/Math/MathTypes.h"

namespace Game {

class Inventory;

enum class EnemyState {
    Idle,
    Patrol,
    Chase,
    Attack,
    Dead
};

class Enemy {
public:
    Enemy();
    Enemy(const std::string& name, const Engine::Vec3& spawnPos, float patrolRadius = 4.0f);

    // Runs state machine ticks. PlayerPos is used for chase triggers.
    // Dt is frame delta. PlayerHealth is passed by reference to apply damage.
    void Update(float dt, const Engine::Vec3& playerPos, float& playerHealth, std::string& outLogMsg);

    // Applies damage to the enemy. Returns true if enemy died on this hit.
    bool TakeDamage(float amount, std::string& outMsg);

    // Getters / Setters
    const std::string& GetName() const { return m_name; }
    Engine::Vec3 GetPosition() const { return m_position; }
    void SetPosition(const Engine::Vec3& pos) { m_position = pos; }

    float GetHealth() const { return m_health; }
    bool IsDead() const { return m_state == EnemyState::Dead; }
    EnemyState GetState() const { return m_state; }

    // Simulates dropping loot into the player's inventory
    void DropLoot(Inventory& playerInv, std::string& outMsg);

private:
    std::string m_name;
    Engine::Vec3 m_position;
    Engine::Vec3 m_spawnOrigin;
    
    float m_health;
    float m_maxHealth;
    float m_damage;
    
    EnemyState m_state;
    float m_patrolRadius;
    float m_patrolTimer;
    Engine::Vec3 m_patrolTarget;

    float m_attackCooldown;
    float m_speed;

    void SetNewPatrolTarget();
};

} // namespace Game
