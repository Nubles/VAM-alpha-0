#include "BuildSystem.h"
#include <algorithm>
#include <cmath>

namespace Game {

BuildSystem::BuildSystem()
    : m_buildModeActive(false),
      m_selectedType(PieceType::Floor),
      m_rotationYaw(0.0f) {}

void BuildSystem::Initialize() {
    m_availablePieces.clear();

    m_availablePieces.push_back({"floor", PieceType::Floor, 4, 0});
    m_availablePieces.push_back({"wall", PieceType::Wall, 3, 2});
    m_availablePieces.push_back({"roof", PieceType::Roof, 0, 3});
}

void BuildSystem::SetSelectedPieceType(PieceType type) {
    m_selectedType = type;
}

void BuildSystem::RotatePreview(float degrees) {
    m_rotationYaw += degrees;
    if (m_rotationYaw >= 360.0f) {
        m_rotationYaw -= 360.0f;
    } else if (m_rotationYaw < 0.0f) {
        m_rotationYaw += 360.0f;
    }
}

const BuildingPiece* BuildSystem::GetSelectedPiece() const {
    for (const auto& piece : m_availablePieces) {
        if (piece.type == m_selectedType) {
            return &piece;
        }
    }
    return nullptr;
}

glm::vec3 BuildSystem::GetPlacementTarget(glm::vec3 cameraPos, glm::vec3 cameraFront) const {
    // Basic raycast trace to flat terrain plane (y = 0)
    // Ray: P(t) = O + t*D
    // Find t where P(t).y = 0 => O.y + t*D.y = 0 => t = -O.y / D.y
    float maxReach = 8.0f;

    if (std::abs(cameraFront.y) > 0.001f) {
        float t = -cameraPos.y / cameraFront.y;
        if (t > 0.0f && t < maxReach) {
            return cameraPos + t * cameraFront;
        }
    }

    // Default target floating forward if no plane intersection matches
    return cameraPos + cameraFront * 4.0f;
}

bool BuildSystem::PlacePiece(Inventory& inv, glm::vec3 position, std::vector<SceneObject>& worldObjects, std::string& outMsg) {
    const BuildingPiece* piece = GetSelectedPiece();
    if (!piece) {
        outMsg = "Selected piece type is invalid!";
        return false;
    }

    // Validate resource costs
    if (piece->woodCost > 0 && !inv.Contains("wood", piece->woodCost)) {
        outMsg = "Missing wood! Requires " + std::to_string(piece->woodCost) + "x Wood.";
        return false;
    }
    if (piece->fiberCost > 0 && !inv.Contains("fiber", piece->fiberCost)) {
        outMsg = "Missing fiber! Requires " + std::to_string(piece->fiberCost) + "x Fiber.";
        return false;
    }

    // Consume materials
    if (piece->woodCost > 0) inv.RemoveItem("wood", piece->woodCost);
    if (piece->fiberCost > 0) inv.RemoveItem("fiber", piece->fiberCost);

    // Create scene object structure representant
    SceneObject structure;
    structure.type = PrimitiveType::Cube;
    structure.transform.position = Engine::Vec3(position.x, position.y, position.z);
    structure.transform.rotation = Engine::Vec3(0.0f, m_rotationYaw, 0.0f);
    
    if (piece->type == PieceType::Floor) {
        structure.name = "Placed Wood Floor";
        structure.color = Engine::Vec3(0.55f, 0.4f, 0.25f); // Wooden floor brown
        structure.transform.scale = Engine::Vec3(2.0f, 0.1f, 2.0f);
        structure.transform.position.y = 0.05f; // Elevate slightly above grid
    } else if (piece->type == PieceType::Wall) {
        structure.name = "Placed Wood Wall";
        structure.color = Engine::Vec3(0.6f, 0.45f, 0.3f);
        structure.transform.scale = Engine::Vec3(2.0f, 1.5f, 0.15f);
        structure.transform.position.y = 0.75f; // Wall height offset
    } else if (piece->type == PieceType::Roof) {
        structure.name = "Placed Fiber Roof";
        structure.color = Engine::Vec3(0.7f, 0.65f, 0.2f); // Thatch yellow
        structure.transform.scale = Engine::Vec3(2.0f, 0.15f, 2.0f);
        structure.transform.position.y = 1.5f; // Ceiling height offset
    }

    worldObjects.push_back(structure);
    outMsg = "Placed " + structure.name + "!";
    return true;
}

} // namespace Game
