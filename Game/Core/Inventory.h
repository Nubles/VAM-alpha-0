#pragma once

#include "ItemStack.h"
#include <vector>

namespace Game {

class Inventory {
public:
    Inventory(int slotCount = 24);

    // Returns the leftover count that could not be added
    int AddItem(const std::string& itemId, int count);
    
    // Checks if inventory contains at least specified count of item
    bool Contains(const std::string& itemId, int count) const;
    
    // Removes specified count of item, returns true on success
    bool RemoveItem(const std::string& itemId, int count);

    const std::vector<ItemStack>& GetSlots() const { return m_slots; }
    int GetSlotCount() const { return static_cast<int>(m_slots.size()); }

private:
    std::vector<ItemStack> m_slots;
};

} // namespace Game
