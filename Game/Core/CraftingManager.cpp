#include "CraftingManager.h"
#include "ItemDatabase.h"
#include <algorithm>

namespace Game {

CraftingManager& CraftingManager::Get() {
    static CraftingManager instance;
    return instance;
}

void CraftingManager::Initialize() {
    m_recipes.clear();

    // Recipe: Primitive Tool
    Recipe toolRecipe;
    toolRecipe.id = "recipe_primitive_tool";
    toolRecipe.displayName = "Primitive Tool";
    toolRecipe.outputItemId = "primitive_tool";
    toolRecipe.outputQuantity = 1;
    toolRecipe.ingredients.push_back({"wood", 5});
    toolRecipe.ingredients.push_back({"stone", 3});
    toolRecipe.ingredients.push_back({"fiber", 2});
    m_recipes.push_back(toolRecipe);

    // Recipe: Processed Fiber Rope (as dummy resource processing recipe)
    Recipe fiberRecipe;
    fiberRecipe.id = "recipe_fiber_rope";
    fiberRecipe.displayName = "Rope Bundle";
    fiberRecipe.outputItemId = "fiber"; // outputs fiber back for simplicity or another dummy
    fiberRecipe.outputQuantity = 2;
    fiberRecipe.ingredients.push_back({"fiber", 4});
    m_recipes.push_back(fiberRecipe);
}

const Recipe* CraftingManager::GetRecipe(const std::string& recipeId) const {
    for (const auto& recipe : m_recipes) {
        if (recipe.id == recipeId) {
            return &recipe;
        }
    }
    return nullptr;
}

bool CraftingManager::CanCraft(const std::string& recipeId, const Inventory& inv) const {
    const Recipe* recipe = GetRecipe(recipeId);
    if (!recipe) return false;

    // Check availability of each ingredient
    for (const auto& ing : recipe->ingredients) {
        if (!inv.Contains(ing.itemId, ing.quantity)) {
            return false;
        }
    }
    return true;
}

bool CraftingManager::Craft(const std::string& recipeId, Inventory& inv, std::string& outMsg) {
    const Recipe* recipe = GetRecipe(recipeId);
    if (!recipe) {
        outMsg = "Unknown recipe!";
        return false;
    }

    if (!CanCraft(recipeId, inv)) {
        outMsg = "Insufficient ingredients to craft " + recipe->displayName + "!";
        return false;
    }

    // Verify if inventory has room for output
    // A simple test check: add output dry run
    // Since Inventory::AddItem tells us leftovers, we can simulate adding or check leftovers
    // We simulate adding to a copy or check slots space
    Inventory testCopy = inv;
    int leftovers = testCopy.AddItem(recipe->outputItemId, recipe->outputQuantity);
    if (leftovers > 0) {
        outMsg = "Inventory is full! Cannot craft " + recipe->displayName + ".";
        return false;
    }

    // Deduct ingredients
    for (const auto& ing : recipe->ingredients) {
        inv.RemoveItem(ing.itemId, ing.quantity);
    }

    // Add output
    inv.AddItem(recipe->outputItemId, recipe->outputQuantity);

    const ItemDefinition* itemDef = ItemDatabase::Get().GetItem(recipe->outputItemId);
    std::string itemName = itemDef ? itemDef->displayName : recipe->outputItemId;
    outMsg = "Crafted " + std::to_string(recipe->outputQuantity) + "x " + itemName + " successfully!";
    return true;
}

} // namespace Game
