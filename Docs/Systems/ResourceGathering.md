# Resource Gathering System

This document outlines the resource gathering mechanics, drop tables, tool check validation, and manual controls configured for Milestone 4.

## Resource Gathering Mechanics

* Raycast-based sphere interaction check aims at resource nodes.
* Focused target prints node status info to player HUD.
* Gathering deduces remaining hit counts, triggers item drops, and checks tools.

## Starter Resource Nodes

1. **Branch Pile** (`"Branch Pile"`):
   - Gives Wood (`wood`, 3-5 quantity).
   - Base interaction radius: 1.2.
   - Initial gather limit: 3 hits.
   - Tool Required: None.
2. **Loose Stone** (`"Loose Stone"`):
   - Gives Stone (`stone`, 2-4 quantity).
   - Base interaction radius: 1.0.
   - Initial gather limit: 2 hits.
   - Tool Required: None.
3. **Fiber Plant** (`"Fiber Plant"`):
   - Gives Fiber (`fiber`, 4-8 quantity).
   - Base interaction radius: 1.0.
   - Initial gather limit: 3 hits.
   - Tool Required: None.
4. **Cracked Rock** (`"Cracked Rock"`):
   - Gives Stone (`stone`, 2-4 quantity).
   - Base interaction radius: 1.8.
   - Initial gather limit: 4 hits.
   - Tool Required: Primitive Tool (`primitive_tool`).

## Tool Requirement Validation

* Gather checks read if inventory contains the required tool ID (e.g. `primitive_tool`).
* Does not reduce durability or consume tool items.
* Gives error logging feedback when target requires a tool and it is not found.

## Intentional Constraints (Not Implemented)

* Respawns / regeneration ticks.
* Durability ticks.
* Physical model swaps (depleted structures remain visible).
