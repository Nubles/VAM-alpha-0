#pragma once

#include <string>

namespace Game {

enum class ItemCategory {
    Resource,
    Tool,
    Food
};

struct ItemDefinition {
    std::string id;
    std::string displayName;
    std::string description;
    ItemCategory category;
    int maxStackSize;
    int resourceTier;
};

} // namespace Game
