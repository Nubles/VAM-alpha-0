#include "../Engine/Math/Transform.h"
#include "../Game/Core/Player.h"
#include <iostream>
#include <cassert>

void TestTransformIdentity() {
    Engine::Transform t;
    Engine::Mat4 m = t.GetModelMatrix();
    
    // Check that default transform is identity matrix
    assert(m[0][0] == 1.0f);
    assert(m[1][1] == 1.0f);
    assert(m[2][2] == 1.0f);
    assert(m[3][3] == 1.0f);
    
    std::cout << "TestTransformIdentity passed!" << std::endl;
}

void TestTransformTranslation() {
    Engine::Transform t;
    t.position = Engine::Vec3(1.0f, 2.0f, 3.0f);
    Engine::Mat4 m = t.GetModelMatrix();
    
    // Check translation elements in the model matrix
    assert(m[3][0] == 1.0f);
    assert(m[3][1] == 2.0f);
    assert(m[3][2] == 3.0f);
    
    std::cout << "TestTransformTranslation passed!" << std::endl;
}

void TestPlayerStamina() {
    Game::Player p;
    // Initial stamina should be max (100)
    assert(p.GetStamina() == 100.0f);

    // Update player when moving and sprinting
    // Stamina should decrease
    p.Update(0.1f, true, true);
    assert(p.GetStamina() < 100.0f);

    // Update player while moving but NOT sprinting
    // Stamina should not regenerate immediately (delay is active)
    float currentStamina = p.GetStamina();
    p.Update(0.5f, true, false);
    assert(p.GetStamina() == currentStamina);

    // Let the delay pass
    p.Update(1.1f, true, false); // total 1.6s delay elapsed
    // Now stamina should begin to regenerate
    p.Update(0.1f, true, false);
    assert(p.GetStamina() > currentStamina);

    std::cout << "TestPlayerStamina passed!" << std::endl;
}

int main() {
    std::cout << "Running EngineCoreTests..." << std::endl;
    TestTransformIdentity();
    TestTransformTranslation();
    TestPlayerStamina();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
