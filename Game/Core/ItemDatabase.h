#pragma once

#include "ItemDefinition.h"
#include <unordered_map>
#include <string>

namespace Game {

class ItemDatabase {
public:
    static ItemDatabase& Get();

    void Initialize();
    const ItemDefinition* GetItem(const std::string& id) const;

private:
    ItemDatabase() = default;
    std::unordered_map<std::string, ItemDefinition> m_items;
};

} // namespace Game
