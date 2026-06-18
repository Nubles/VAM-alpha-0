#include "Hotbar.h"

namespace Game {

Hotbar::Hotbar(int size)
    : m_activeIndex(0) {
    m_inventoryIndices.resize(size);
    // Bind hotbar slot 0..7 to inventory slots 0..7
    for (int i = 0; i < size; ++i) {
        m_inventoryIndices[i] = i;
    }
}

void Hotbar::SelectSlot(int index) {
    if (index >= 0 && index < static_cast<int>(m_inventoryIndices.size())) {
        m_activeIndex = index;
    }
}

void Hotbar::SelectNext() {
    m_activeIndex = (m_activeIndex + 1) % static_cast<int>(m_inventoryIndices.size());
}

void Hotbar::SelectPrevious() {
    m_activeIndex = (m_activeIndex - 1 + static_cast<int>(m_inventoryIndices.size())) % static_cast<int>(m_inventoryIndices.size());
}

int Hotbar::GetInventoryIndex(int hotbarIndex) const {
    if (hotbarIndex >= 0 && hotbarIndex < static_cast<int>(m_inventoryIndices.size())) {
        return m_inventoryIndices[hotbarIndex];
    }
    return -1;
}

void Hotbar::SetInventoryIndex(int hotbarIndex, int inventorySlotIndex) {
    if (hotbarIndex >= 0 && hotbarIndex < static_cast<int>(m_inventoryIndices.size())) {
        m_inventoryIndices[hotbarIndex] = inventorySlotIndex;
    }
}

} // namespace Game
