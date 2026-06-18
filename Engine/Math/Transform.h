#pragma once

#include "MathTypes.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

struct Transform {
    Vec3 position = Vec3(0.0f);
    Vec3 rotation = Vec3(0.0f); // Euler rotation angles in degrees (x: pitch, y: yaw, z: roll)
    Vec3 scale    = Vec3(1.0f);

    Mat4 GetModelMatrix() const {
        Mat4 model = Mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), Vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), Vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), Vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }
};

} // namespace Engine
