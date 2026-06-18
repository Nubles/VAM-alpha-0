#include "ItemDatabase.h"

namespace Game {

ItemDatabase& ItemDatabase::Get() {
    static ItemDatabase instance;
    return instance;
}

void ItemDatabase::Initialize() {
    m_items.clear();

    m_items["wood"] = {
        "wood", "Wood", "Sturdy branch usable for crafting basic tools.",
        ItemCategory::Resource, 50, 1
    };

    m_items["stone"] = {
        "stone", "Stone", "A smooth, heavy rock found scattered on the ground.",
        ItemCategory::Resource, 50, 1
    };

    m_items["fiber"] = {
        "fiber", "Fiber", "Plant fibers harvested from weeds and bushes.",
        ItemCategory::Resource, 100, 1
    };

    m_items["raw_food"] = {
        "raw_food", "Raw Food", "Wild berries and mushrooms. Safe to eat, but basic.",
        ItemCategory::Food, 20, 1
    };

    m_items["primitive_tool"] = {
        "primitive_tool", "Primitive Tool", "A basic stone hatchet, useful for chipping rocky structures.",
        ItemCategory::Tool, 1, 1
    };
}

const ItemDefinition* ItemDatabase::GetItem(const std::string& id) const {
    auto it = m_items.find(id);
    if (it != m_items.end()) {
        return &it->second;
    }
    return nullptr;
}

} // namespace Game
