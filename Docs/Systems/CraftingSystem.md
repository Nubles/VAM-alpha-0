# Crafting System

This document outlines the crafting mechanics, recipes, inventory integration, and manual controls configured for Milestone 5.

## Crafting Mechanics

* The Workbench & Crafting UI is rendered inside ImGui.
* Allows players to inspect recipes, check ingredients availability, and craft items.
* Crafting performs validation (checking materials and inventory slot space), consumes materials, and grants output items.

## Recipe Definitions

1. **Primitive Tool** (`"recipe_primitive_tool"`):
   - Output: 1x Primitive Tool (`primitive_tool`).
   - Ingredients Required:
     - 5x Wood (`wood`)
     - 3x Stone (`stone`)
     - 2x Fiber (`fiber`)
2. **Rope Bundle** (`"recipe_fiber_rope"`):
   - Output: 2x Fiber (`fiber`, dummy recipe processing example).
   - Ingredients Required:
     - 4x Fiber (`fiber`)

## Inventory Integration

* Validates ingredients counts across inventory slots before executing.
* Deducts ingredients and frees up stack/slot space.
* Simulates product injection to prevent resource consumption if inventory slots are full.

## Intentional Constraints (Not Implemented)

* Specialized station interactions (requiring standing near workbenches/forges).
* Durability consumption or material degradation.
* Queue timelines (crafting executes instantly).
