#pragma once

#include <string>
#include <vector>

namespace Game {

struct DropEntry {
    std::string itemId;
    int minQty;
    int maxQty;
};

struct DropTable {
    std::vector<DropEntry> entries;

    // Returns a list of generated item drops (item ID, quantity)
    std::vector<std::pair<std::string, int>> Roll() const {
        std::vector<std::pair<std::string, int>> results;
        for (const auto& entry : entries) {
            // Deterministic/simple distribution: return minQty for simplicity
            int qty = entry.minQty;
            if (qty > 0) {
                results.push_back({entry.itemId, qty});
            }
        }
        return results;
    }
};

} // namespace Game
