#pragma once

#include <glm/glm.hpp>

namespace Engine {

// Aliases for GLM math types to decouple engine calls from the raw library
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Mat4 = glm::mat4;

} // namespace Engine
