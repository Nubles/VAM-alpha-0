#pragma once

#include "BuildingPiece.h"
#include "Inventory.h"
#include "SceneObject.h"
#include <vector>
#include <string>

namespace Game {

class BuildSystem {
public:
    BuildSystem();

    void Initialize();

    bool IsBuildMode() const { return m_buildModeActive; }
    void ToggleBuildMode() { m_buildModeActive = !m_buildModeActive; }
    void SetBuildMode(bool active) { m_buildModeActive = active; }

    PieceType GetSelectedPieceType() const { return m_selectedType; }
    void SetSelectedPieceType(PieceType type);

    float GetRotationYaw() const { return m_rotationYaw; }
    void RotatePreview(float degrees);

    // Determines the intersection trace point on a flat plane (y=0) or nearest surfaces
    glm::vec3 GetPlacementTarget(glm::vec3 cameraPos, glm::vec3 cameraFront) const;

    // Checks resources, consumes them, and appends structure primitive to the scene list
    bool PlacePiece(Inventory& inv, glm::vec3 position, std::vector<SceneObject>& worldObjects, std::string& outMsg);

    const BuildingPiece* GetSelectedPiece() const;

private:
    bool m_buildModeActive;
    PieceType m_selectedType;
    float m_rotationYaw;

    std::vector<BuildingPiece> m_availablePieces;
};

} // namespace Game
