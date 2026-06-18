#include "GatherableNode.h"
#include "ItemDatabase.h"

namespace Game {

GatherableNode::GatherableNode(const std::string& name, const DropTable& dropTable, int maxGatherHits, const std::string& requiredToolId)
    : m_name(name),
      m_dropTable(dropTable),
      m_maxGatherHits(maxGatherHits),
      m_remainingGatherHits(maxGatherHits),
      m_requiredToolId(requiredToolId) {}

bool GatherableNode::Gather(Inventory& inv, std::string& outMsg) {
    if (IsDepleted()) {
        outMsg = m_name + " is depleted!";
        return false;
    }

    // Check required tool in inventory
    if (!m_requiredToolId.empty() && !inv.Contains(m_requiredToolId, 1)) {
        const ItemDefinition* toolDef = ItemDatabase::Get().GetItem(m_requiredToolId);
        std::string toolName = toolDef ? toolDef->displayName : m_requiredToolId;
        outMsg = "Requires " + toolName + "!";
        return false;
    }

    // Roll drops
    auto drops = m_dropTable.Roll();
    if (drops.empty()) {
        outMsg = "Gathered nothing from " + m_name + ".";
        return false;
    }

    // Check if inventory can fit drops
    bool addedAny = false;
    std::string dropMsg;

    for (const auto& drop : drops) {
        int leftover = inv.AddItem(drop.first, drop.second);
        int added = drop.second - leftover;

        if (added > 0) {
            addedAny = true;
            const ItemDefinition* itemDef = ItemDatabase::Get().GetItem(drop.first);
            std::string itemName = itemDef ? itemDef->displayName : drop.first;
            if (!dropMsg.empty()) dropMsg += ", ";
            dropMsg += "+" + std::to_string(added) + " " + itemName;
            if (leftover > 0) {
                dropMsg += " (" + std::to_string(leftover) + " leftover)";
            }
        }
    }

    if (!addedAny) {
        outMsg = "Inventory is full! Cannot gather " + m_name + ".";
        return false;
    }

    // Deduct gather hits
    m_remainingGatherHits--;
    outMsg = "Gathered " + m_name + ": " + dropMsg;
    if (IsDepleted()) {
        outMsg += " [Depleted]";
    }
    return true;
}

} // namespace Game
