#pragma once

#include <string>

namespace Game {

enum class PieceType {
    Floor,
    Wall,
    Roof
};

struct BuildingPiece {
    std::string id;
    PieceType type;
    int woodCost;
    int fiberCost;
};

} // namespace Game
