#pragma once

#include "Recipe.h"
#include "Inventory.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace Game {

class CraftingManager {
public:
    static CraftingManager& Get();

    void Initialize();
    
    // Checks if the player inventory contains sufficient ingredients to craft
    bool CanCraft(const std::string& recipeId, const Inventory& inv) const;
    
    // Attempts to craft the item, deducts ingredients, adds output. Fills status in outMsg
    bool Craft(const std::string& recipeId, Inventory& inv, std::string& outMsg);

    const std::vector<Recipe>& GetRecipes() const { return m_recipes; }
    const Recipe* GetRecipe(const std::string& recipeId) const;

private:
    CraftingManager() = default;
    std::vector<Recipe> m_recipes;
};

} // namespace Game
