#pragma once

#include <vector>

namespace Game {

class Hotbar {
public:
    Hotbar(int size = 8);

    void SelectSlot(int index);
    void SelectNext();
    void SelectPrevious();

    int GetActiveIndex() const { return m_activeIndex; }
    int GetSlotCount() const { return static_cast<int>(m_inventoryIndices.size()); }
    int GetInventoryIndex(int hotbarIndex) const;
    void SetInventoryIndex(int hotbarIndex, int inventorySlotIndex);

private:
    int m_activeIndex;
    std::vector<int> m_inventoryIndices;
};

} // namespace Game
