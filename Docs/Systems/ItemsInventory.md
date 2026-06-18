# Data-Driven Items & Inventory System

This document outlines the data-driven items, database config, slot/stack merging behavior, hotbar model, and manual controls configured for Milestone 3.

## Starter Items List

* **Wood** (`"wood"`): Max stack 50. Category: Resource.
* **Stone** (`"stone"`): Max stack 50. Category: Resource.
* **Fiber** (`"fiber"`): Max stack 100. Category: Resource.
* **Raw Food** (`"raw_food"`): Max stack 20. Category: Food.
* **Primitive Tool** (`"primitive_tool"`): Max stack 1. Category: Tool.

## Inventory Slots & Stacking Behavior

* **Inventory Size**: 24 slots total.
* **Stacking Rules**:
  - Item insertions merge into existing identical stacks up to the item's `maxStackSize`.
  - Spills and new items allocate the first available empty slot.
  - Overflow quantities exceeding total capacity are returned.
* **Removal**: Standard logic checking existence (`Contains`) and subtracting counts across stacks starting from partial stacks first.

## Hotbar Configuration

* **Hotbar Size**: 8 slots.
* **Selection**: Points to the first 8 inventory slots. 
* **Key Binds**: Pressing numbers `1` through `8` sets active selection.

## Debug Commands

* **F1**: Grants 10x Wood.
* **F2**: Grants 10x Stone.
* **F3**: Grants 15x Fiber.
* **F4**: Grants 5x Raw Food.
* **F5**: Grants 1x Primitive Tool.

## Intentional Constraints (Not Implemented)

* Durability loss or damage properties on tools.
* Drag-and-drop slots UI.
* Equipment slot types (Armor, active hands slots, etc.).
* World drops/spawns rendering.
