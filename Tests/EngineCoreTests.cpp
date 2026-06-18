#include "../Engine/Math/Transform.h"
#include "../Game/Core/Player.h"
#include "../Game/Core/ItemDatabase.h"
#include "../Game/Core/Inventory.h"
#include "../Game/Core/Hotbar.h"
#include "../Game/Core/DropTable.h"
#include "../Game/Core/GatherableNode.h"
#include "../Game/Core/Recipe.h"
#include "../Game/Core/CraftingManager.h"
#include "../Game/Core/BuildingPiece.h"
#include "../Game/Core/BuildSystem.h"
#include <iostream>
#include <cassert>

void TestTransformIdentity() {
    Engine::Transform t;
    Engine::Mat4 m = t.GetModelMatrix();
    
    // Check that default transform is identity matrix
    assert(m[0][0] == 1.0f);
    assert(m[1][1] == 1.0f);
    assert(m[2][2] == 1.0f);
    assert(m[3][3] == 1.0f);
    
    std::cout << "TestTransformIdentity passed!" << std::endl;
}

void TestTransformTranslation() {
    Engine::Transform t;
    t.position = Engine::Vec3(1.0f, 2.0f, 3.0f);
    Engine::Mat4 m = t.GetModelMatrix();
    
    // Check translation elements in the model matrix
    assert(m[3][0] == 1.0f);
    assert(m[3][1] == 2.0f);
    assert(m[3][2] == 3.0f);
    
    std::cout << "TestTransformTranslation passed!" << std::endl;
}

void TestPlayerStamina() {
    Game::Player p;
    // Initial stamina should be max (100)
    assert(p.GetStamina() == 100.0f);

    // Update player when moving and sprinting
    // Stamina should decrease
    p.Update(0.1f, true, true);
    assert(p.GetStamina() < 100.0f);

    // Update player while moving but NOT sprinting
    // Stamina should not regenerate immediately (delay is active)
    float currentStamina = p.GetStamina();
    p.Update(0.5f, true, false);
    assert(p.GetStamina() == currentStamina);

    // Let the delay pass
    p.Update(1.1f, true, false); // total 1.6s delay elapsed
    // Now stamina should begin to regenerate
    p.Update(0.1f, true, false);
    assert(p.GetStamina() > currentStamina);

    std::cout << "TestPlayerStamina passed!" << std::endl;
}

void TestInventoryAndHotbar() {
    // Initialize Item Database
    Game::ItemDatabase::Get().Initialize();

    // 1. Definition lookup
    const Game::ItemDefinition* woodDef = Game::ItemDatabase::Get().GetItem("wood");
    assert(woodDef != nullptr);
    assert(woodDef->displayName == "Wood");
    assert(woodDef->maxStackSize == 50);

    const Game::ItemDefinition* invalidDef = Game::ItemDatabase::Get().GetItem("invalid_item");
    assert(invalidDef == nullptr);

    // 2. Inventory slot size & initial empty state
    Game::Inventory inv(24);
    assert(inv.GetSlotCount() == 24);
    assert(inv.Contains("wood", 1) == false);

    // 3. Stacking items
    int leftover = inv.AddItem("wood", 20);
    assert(leftover == 0);
    assert(inv.Contains("wood", 20) == true);
    assert(inv.Contains("wood", 21) == false);

    // Verify slots contents
    const auto& slots = inv.GetSlots();
    assert(slots[0].itemId == "wood");
    assert(slots[0].count == 20);

    // 4. Stacking and merging
    leftover = inv.AddItem("wood", 40); // 20 + 40 = 60 items. Max stack is 50.
    assert(leftover == 0); // 0 leftovers because remaining 10 items spill into slot 1
    assert(slots[0].count == 50);
    assert(slots[1].itemId == "wood");
    assert(slots[1].count == 10);
    
    // Let's clear inventory and do a strict test
    Game::Inventory testInv(1); // 1 slot inventory
    leftover = testInv.AddItem("wood", 20);
    assert(leftover == 0);
    leftover = testInv.AddItem("wood", 40); // adds 30 to reach 50, returns 10 leftover
    assert(leftover == 10);
    assert(testInv.GetSlots()[0].count == 50);

    // 5. Deletion & Removal
    Game::Inventory removeInv(5);
    removeInv.AddItem("stone", 10);
    assert(removeInv.Contains("stone", 10));
    bool success = removeInv.RemoveItem("stone", 6);
    assert(success);
    assert(removeInv.Contains("stone", 4));
    assert(!removeInv.Contains("stone", 5));

    // Try to remove too many
    success = removeInv.RemoveItem("stone", 10);
    assert(!success);
    assert(removeInv.Contains("stone", 4)); // count remains unchanged if it fails

    // 6. Hotbar selection
    Game::Hotbar hb(8);
    assert(hb.GetActiveIndex() == 0);
    hb.SelectNext();
    assert(hb.GetActiveIndex() == 1);
    hb.SelectPrevious();
    assert(hb.GetActiveIndex() == 0);
    hb.SelectSlot(5);
    assert(hb.GetActiveIndex() == 5);

    std::cout << "TestInventoryAndHotbar passed!" << std::endl;
}

void TestResourceGathering() {
    Game::ItemDatabase::Get().Initialize();

    // 1. DropTable distribution test
    Game::DropTable woodTable;
    woodTable.entries.push_back({"wood", 5, 10});
    auto drops = woodTable.Roll();
    assert(drops.size() == 1);
    assert(drops[0].first == "wood");
    assert(drops[0].second == 5);

    // 2. Gatherable Node setup
    Game::GatherableNode branchNode("Branch Pile", woodTable, 2);
    assert(branchNode.GetName() == "Branch Pile");
    assert(!branchNode.IsDepleted());
    assert(branchNode.GetRemainingHits() == 2);

    Game::Inventory inv(5);
    std::string msg;

    // 3. Gather without tools requirements
    bool success = branchNode.Gather(inv, msg);
    assert(success);
    assert(inv.Contains("wood", 5));
    assert(branchNode.GetRemainingHits() == 1);

    // 4. Node depletion check
    success = branchNode.Gather(inv, msg);
    assert(success);
    assert(branchNode.IsDepleted());
    assert(branchNode.GetRemainingHits() == 0);

    // Gather when depleted should fail
    success = branchNode.Gather(inv, msg);
    assert(!success);

    // 5. Gather with tool requirements
    Game::DropTable stoneTable;
    stoneTable.entries.push_back({"stone", 3, 5});
    Game::GatherableNode rockNode("Cracked Rock", stoneTable, 2, "primitive_tool");
    assert(rockNode.GetRequiredToolId() == "primitive_tool");

    Game::Inventory toolInv(5);
    // Attempt gather without tool in inventory (should fail)
    success = rockNode.Gather(toolInv, msg);
    assert(!success);
    assert(!toolInv.Contains("stone", 1));

    // Grant primitive tool and attempt gather (should succeed)
    toolInv.AddItem("primitive_tool", 1);
    success = rockNode.Gather(toolInv, msg);
    assert(success);
    assert(toolInv.Contains("stone", 3));

    // 6. Overflow handling
    Game::Inventory fullInv(1);
    // Fill inventory completely
    fullInv.AddItem("fiber", 100); // Max stack size is 100

    Game::DropTable fiberTable;
    fiberTable.entries.push_back({"fiber", 10, 20});
    Game::GatherableNode plantNode("Fiber Plant", fiberTable, 2);

    // Attempt gather should fail because inventory has no space
    success = plantNode.Gather(fullInv, msg);
    assert(!success);

    std::cout << "TestResourceGathering passed!" << std::endl;
}

void TestCraftingSystem() {
    Game::ItemDatabase::Get().Initialize();
    Game::CraftingManager::Get().Initialize();

    // 1. Recipe verification
    const Game::Recipe* toolRecipe = Game::CraftingManager::Get().GetRecipe("recipe_primitive_tool");
    assert(toolRecipe != nullptr);
    assert(toolRecipe->displayName == "Primitive Tool");
    assert(toolRecipe->ingredients[0].itemId == "wood");
    assert(toolRecipe->ingredients[0].quantity == 5);

    // 2. CanCraft checks
    Game::Inventory inv(5);
    // Add partial ingredients
    inv.AddItem("wood", 4);
    inv.AddItem("stone", 3);
    inv.AddItem("fiber", 2);

    // Should fail (missing 1 wood)
    assert(!Game::CraftingManager::Get().CanCraft("recipe_primitive_tool", inv));

    // Add remaining wood
    inv.AddItem("wood", 1);
    // Should succeed
    assert(Game::CraftingManager::Get().CanCraft("recipe_primitive_tool", inv));

    // 3. Execution of Craft
    std::string msg;
    bool success = Game::CraftingManager::Get().Craft("recipe_primitive_tool", inv, msg);
    assert(success);
    
    // Verify ingredients consumed: wood (5-5=0), stone (3-3=0), fiber (2-2=0)
    assert(!inv.Contains("wood", 1));
    assert(!inv.Contains("stone", 1));
    assert(!inv.Contains("fiber", 1));
    
    // Verify output tool added to inventory
    assert(inv.Contains("primitive_tool", 1));

    // 4. Inventory full crafting check
    Game::Inventory fullInv(1);
    fullInv.AddItem("wood", 50); // Single slot filled completely with wood

    // Setup inventory with materials but no space for output tool
    Game::Inventory noSpaceInv(3);
    noSpaceInv.AddItem("wood", 5);
    noSpaceInv.AddItem("stone", 3);
    noSpaceInv.AddItem("fiber", 2);
    // Fill the remaining slots completely with fiber
    noSpaceInv.AddItem("fiber", 98); // consumes 2 fiber, slot 2 has 96 fiber. Inventory has no empty slots left.
    // Tool requires 1 slot. So crafting should fail.
    success = Game::CraftingManager::Get().Craft("recipe_primitive_tool", noSpaceInv, msg);
    assert(!success);

    std::cout << "TestCraftingSystem passed!" << std::endl;
}

void TestBuildingPlacement() {
    Game::BuildSystem bs;
    bs.Initialize();

    // 1. Selection & Mode tracking
    assert(!bs.IsBuildMode());
    bs.ToggleBuildMode();
    assert(bs.IsBuildMode());

    assert(bs.GetSelectedPieceType() == Game::PieceType::Floor);
    bs.SetSelectedPieceType(Game::PieceType::Wall);
    assert(bs.GetSelectedPieceType() == Game::PieceType::Wall);

    // 2. Rotation checks
    assert(bs.GetRotationYaw() == 0.0f);
    bs.RotatePreview(45.0f);
    assert(bs.GetRotationYaw() == 45.0f);
    bs.RotatePreview(320.0f);
    assert(bs.GetRotationYaw() == 5.0f); // 365 - 360 = 5

    // 3. Cost checking validation fails when resources missing
    Game::Inventory inv(5);
    std::vector<Game::SceneObject> worldObjects;
    std::string msg;

    // Floor costs 4 wood
    bs.SetSelectedPieceType(Game::PieceType::Floor);
    bool success = bs.PlacePiece(inv, glm::vec3(0, 0, 0), worldObjects, msg);
    assert(!success);
    assert(worldObjects.empty());

    // 4. Successful placement consumes items and registers world mesh
    inv.AddItem("wood", 10);
    success = bs.PlacePiece(inv, glm::vec3(1, 2, 3), worldObjects, msg);
    assert(success);
    assert(inv.Contains("wood", 6));
    assert(worldObjects.size() == 1);
    assert(worldObjects[0].name == "Placed Wood Floor");
    assert(worldObjects[0].transform.position.x == 1.0f);
    assert(worldObjects[0].transform.position.z == 3.0f);

    std::cout << "TestBuildingPlacement passed!" << std::endl;
}

int main() {
    std::cout << "Running EngineCoreTests..." << std::endl;
    TestTransformIdentity();
    TestTransformTranslation();
    TestPlayerStamina();
    TestInventoryAndHotbar();
    TestResourceGathering();
    TestCraftingSystem();
    TestBuildingPlacement();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
