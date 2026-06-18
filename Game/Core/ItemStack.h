#pragma once

#include <string>

namespace Game {

struct ItemStack {
    std::string itemId;
    int count = 0;

    bool IsEmpty() const { return itemId.empty() || count <= 0; }
    void Clear() {
        itemId.clear();
        count = 0;
    }
};

} // namespace Game
