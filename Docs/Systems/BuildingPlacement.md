# Building Placement System

This document outlines the building placement system mechanics, piece types, material cost rules, and manual controls configured for Milestone 6.

## Building Placement Mechanics

* **Toggle Build Mode**: Toggled using the `B` key (locks/unlocks building placement state).
* **Placement Preview (Ghost Mesh)**: When active, projects a raycast trace from the player's camera position forward to target the terrain plane coordinates. A temporary blue wireframe box represents the ghost model preview.
* **Rotate Piece**: Use the Arrow keys `LEFT`/`RIGHT` to rotate the ghost piece yaw angle.
* **Place Structure**: Click Left Mouse Button in the sandbox. Consumes required inventory items and spawns a permanent visual SceneObject mesh.

## Structural Pieces List

1. **Wood Floor** (`PieceType::Floor`):
   - Cost: 4x Wood (`wood`).
   - Transform Scale: `(2.0, 0.1, 2.0)`.
   - Mesh Color: Wooden brown (`0.55, 0.4, 0.25`).
2. **Wood Wall** (`PieceType::Wall`):
   - Cost: 3x Wood (`wood`), 2x Fiber (`fiber`).
   - Transform Scale: `(2.0, 1.5, 0.15)`.
   - Mesh Color: Wall brown (`0.6, 0.45, 0.3`).
3. **Fiber Roof** (`PieceType::Roof`):
   - Cost: 3x Fiber (`fiber`).
   - Transform Scale: `(2.0, 0.15, 2.0)`.
   - Mesh Color: Thatch yellow (`0.7, 0.65, 0.2`).

## Inventory Cost Deduction

* Placing a piece checks the cost. If insufficient resources exist, placement fails and registers a notification log message.
* If materials match, counts are deducted, and the mesh is instantiated.

## Intentional Constraints (Not Implemented)

* Snapping mechanics (free-form placement along Targeted intersection trace only).
* Stability checks or support calculations (pieces float in mid-air if targeted there).
* Structural integrity health decay.
