#pragma once

#include "../../Engine/Math/Transform.h"
#include <string>

namespace Game {

enum class PrimitiveType {
    Cube,
    Grid
};

struct SceneObject {
    std::string name;
    Engine::Transform transform;
    PrimitiveType type = PrimitiveType::Cube;
    Engine::Vec3 color = Engine::Vec3(1.0f);
    // Interaction System properties
    bool isInteractable = false;
    float interactionRadius = 2.0f;
    std::string interactionMessage = "Interacted!";
};

} // namespace Game
