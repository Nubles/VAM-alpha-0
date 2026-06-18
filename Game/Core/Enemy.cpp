#include "Enemy.h"
#include "Inventory.h"
#include <cmath>
#include <algorithm>
#include <cstdlib>

namespace Game {

Enemy::Enemy()
    : m_name("Ironbound Sentinel"),
      m_position(Engine::Vec3(0.0f)),
      m_spawnOrigin(Engine::Vec3(0.0f)),
      m_health(100.0f),
      m_maxHealth(100.0f),
      m_damage(10.0f),
      m_state(EnemyState::Patrol),
      m_patrolRadius(4.0f),
      m_patrolTimer(0.0f),
      m_attackCooldown(0.0f),
      m_speed(1.5f) {
    m_patrolTarget = m_position;
}

Enemy::Enemy(const std::string& name, const Engine::Vec3& spawnPos, float patrolRadius)
    : m_name(name),
      m_position(spawnPos),
      m_spawnOrigin(spawnPos),
      m_health(100.0f),
      m_maxHealth(100.0f),
      m_damage(10.0f),
      m_state(EnemyState::Patrol),
      m_patrolRadius(patrolRadius),
      m_patrolTimer(0.0f),
      m_attackCooldown(0.0f),
      m_speed(1.5f) {
    m_patrolTarget = spawnPos;
}

void Enemy::SetNewPatrolTarget() {
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
    float dist = static_cast<float>(rand()) / RAND_MAX * m_patrolRadius;
    m_patrolTarget = m_spawnOrigin + Engine::Vec3(std::cos(angle) * dist, 0.0f, std::sin(angle) * dist);
}

void Enemy::Update(float dt, const Engine::Vec3& playerPos, float& playerHealth, std::string& outLogMsg) {
    if (m_state == EnemyState::Dead) return;

    // Tick cooldowns
    if (m_attackCooldown > 0.0f) {
        m_attackCooldown -= dt;
    }

    // Compute distance to player
    float dx = playerPos.x - m_position.x;
    float dy = playerPos.y - m_position.y;
    float dz = playerPos.z - m_position.z;
    float distToPlayer = std::sqrt(dx * dx + dy * dy + dz * dz);

    // AI State Transitions
    if (distToPlayer <= 1.5f) {
        m_state = EnemyState::Attack;
    } else if (distToPlayer <= 6.0f) {
        m_state = EnemyState::Chase;
    } else {
        if (m_state == EnemyState::Chase || m_state == EnemyState::Attack) {
            m_state = EnemyState::Patrol;
            m_patrolTarget = m_position;
        }
    }

    // Actions
    if (m_state == EnemyState::Attack) {
        if (m_attackCooldown <= 0.0f && playerHealth > 0.0f) {
            playerHealth -= m_damage;
            if (playerHealth < 0.0f) playerHealth = 0.0f;
            m_attackCooldown = 2.0f; // Attack rate 2.0 seconds
            outLogMsg = m_name + " attacked you for " + std::to_string(static_cast<int>(m_damage)) + " damage!";
        }
    } else if (m_state == EnemyState::Chase) {
        // Steer towards player
        Engine::Vec3 dir(dx, dy, dz);
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (len > 0.0f) {
            dir.x /= len; dir.y /= len; dir.z /= len;
        }
        m_position.x += dir.x * m_speed * dt;
        m_position.z += dir.z * m_speed * dt;
    } else if (m_state == EnemyState::Patrol) {
        float tx = m_patrolTarget.x - m_position.x;
        float tz = m_patrolTarget.z - m_position.z;
        float distToTarget = std::sqrt(tx * tx + tz * tz);

        if (distToTarget < 0.2f) {
            m_patrolTimer += dt;
            if (m_patrolTimer >= 3.0f) { // Pause 3s at nodes
                SetNewPatrolTarget();
                m_patrolTimer = 0.0f;
            }
        } else {
            Engine::Vec3 dir(tx, 0.0f, tz);
            if (distToTarget > 0.0f) {
                dir.x /= distToTarget;
                dir.z /= distToTarget;
            }
            m_position.x += dir.x * (m_speed * 0.6f) * dt;
            m_position.z += dir.z * (m_speed * 0.6f) * dt;
        }
    }
}

bool Enemy::TakeDamage(float amount, std::string& outMsg) {
    if (m_state == EnemyState::Dead) return false;

    m_health -= amount;
    outMsg = "Hit " + m_name + " for " + std::to_string(static_cast<int>(amount)) + " damage! (Health: " + std::to_string(static_cast<int>(m_health)) + "/100)";
    if (m_health <= 0.0f) {
        m_health = 0.0f;
        m_state = EnemyState::Dead;
        outMsg += " - " + m_name + " defeated!";
        return true;
    }
    return false;
}

void Enemy::DropLoot(Inventory& playerInv, std::string& outMsg) {
    // Drop Stone and Raw Food
    int stoneLeft = playerInv.AddItem("stone", 3);
    int foodLeft = playerInv.AddItem("raw_food", 1);
    
    outMsg = "Loot Dropped: +3 Stone";
    if (stoneLeft > 0) outMsg += " (" + std::to_string(stoneLeft) + " leftover)";
    outMsg += ", +1 Raw Food";
    if (foodLeft > 0) outMsg += " (" + std::to_string(foodLeft) + " leftover)";
}

} // namespace Game
