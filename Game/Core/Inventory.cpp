#include "Inventory.h"
#include "ItemDatabase.h"
#include <algorithm>

namespace Game {

Inventory::Inventory(int slotCount) {
    m_slots.resize(slotCount);
}

int Inventory::AddItem(const std::string& itemId, int count) {
    const ItemDefinition* def = ItemDatabase::Get().GetItem(itemId);
    if (!def || count <= 0) return count;

    int remaining = count;

    // 1. Merge into existing slots first
    for (auto& slot : m_slots) {
        if (slot.itemId == itemId) {
            int space = def->maxStackSize - slot.count;
            if (space > 0) {
                int toAdd = std::min(space, remaining);
                slot.count += toAdd;
                remaining -= toAdd;
                if (remaining <= 0) return 0;
            }
        }
    }

    // 2. Add to empty slots
    for (auto& slot : m_slots) {
        if (slot.IsEmpty()) {
            slot.itemId = itemId;
            int toAdd = std::min(def->maxStackSize, remaining);
            slot.count = toAdd;
            remaining -= toAdd;
            if (remaining <= 0) return 0;
        }
    }

    return remaining;
}

bool Inventory::Contains(const std::string& itemId, int count) const {
    if (count <= 0) return true;
    int found = 0;
    for (const auto& slot : m_slots) {
        if (slot.itemId == itemId) {
            found += slot.count;
            if (found >= count) return true;
        }
    }
    return false;
}

bool Inventory::RemoveItem(const std::string& itemId, int count) {
    if (!Contains(itemId, count)) return false;

    int toRemove = count;
    // Iterate backwards to remove from partial stacks first or just sequential
    for (auto it = m_slots.rbegin(); it != m_slots.rend(); ++it) {
        if (it->itemId == itemId) {
            if (it->count >= toRemove) {
                it->count -= toRemove;
                if (it->count == 0) {
                    it->Clear();
                }
                return true;
            } else {
                toRemove -= it->count;
                it->Clear();
            }
        }
    }
    return toRemove <= 0;
}

} // namespace Game
