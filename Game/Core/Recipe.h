#pragma once

#include <string>
#include <vector>

namespace Game {

struct Ingredient {
    std::string itemId;
    int quantity;
};

struct Recipe {
    std::string id;
    std::string displayName;
    std::string outputItemId;
    int outputQuantity;
    std::vector<Ingredient> ingredients;
};

} // namespace Game
