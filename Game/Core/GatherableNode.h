#pragma once

#include "DropTable.h"
#include "Inventory.h"
#include <string>

namespace Game {

class GatherableNode {
public:
    GatherableNode() = default;
    GatherableNode(const std::string& name, const DropTable& dropTable, int maxGatherHits = 3, const std::string& requiredToolId = "");

    // Attempts to gather from this node into the inventory.
    // Returns true if gathering was successful.
    // Fills outMsg with result details (success, tools missing, full, depleted).
    bool Gather(Inventory& inv, std::string& outMsg);

    // Getters & Setters
    const std::string& GetName() const { return m_name; }
    bool IsDepleted() const { return m_remainingGatherHits <= 0; }
    int GetRemainingHits() const { return m_remainingGatherHits; }
    const std::string& GetRequiredToolId() const { return m_requiredToolId; }

private:
    std::string m_name;
    DropTable m_dropTable;
    int m_maxGatherHits = 3;
    int m_remainingGatherHits = 3;
    std::string m_requiredToolId;
};

} // namespace Game
