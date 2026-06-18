#include "../Engine/Math/Transform.h"
#include "../Game/Core/Player.h"
#include "../Game/Core/ItemDatabase.h"
#include "../Game/Core/Inventory.h"
#include "../Game/Core/Hotbar.h"
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

int main() {
    std::cout << "Running EngineCoreTests..." << std::endl;
    TestTransformIdentity();
    TestTransformTranslation();
    TestPlayerStamina();
    TestInventoryAndHotbar();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
