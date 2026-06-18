#include "../Engine/Math/Transform.h"
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

int main() {
    std::cout << "Running EngineCoreTests..." << std::endl;
    TestTransformIdentity();
    TestTransformTranslation();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
