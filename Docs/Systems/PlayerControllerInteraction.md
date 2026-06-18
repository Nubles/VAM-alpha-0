# Player Controller & Interaction System

This document outlines the controls, vitals behavior, interaction raycast system, and intentional constraints implemented for Milestone 2.

## Controls

* **WASD**: Move player camera view forward, backward, left, and right.
* **Mouse**: Look around (captured on left-click, released using Left-Alt).
* **Left Shift**: Sprint (increases speed to 10.0f from base 5.0f, drains stamina).
* **E**: Interact with looked-at interactables.
* **Escape**: Quit game client.

## Health & Stamina System

* **Health**: Set to a static 100.0% starting value.
* **Stamina**: Begins at 100.0%. Sprinting consumes `25.0/second`.
* **Regeneration Delay**: Releases regeneration after `1.5` seconds of non-sprinting state.
* **Regeneration Rate**: Recovers at `15.0/second` after regeneration delay threshold passes.

## Interaction System

* Interaction is calculated via a forward-facing camera ray check.
* Sphere overlap check finds the closest looking-at interactable target object within `4.0` units.
* If a target is aimed at, HUD shows: `Target: [Interactable Name]`.
* Pressing `E` key outputs logs in both stdout console and the last interaction log box.

## Intentional Constraints (Not Implemented)

* Durability / Resource nodes depletion.
* Inventory, Items, and Crafting logic.
* Physics engine collision resolving (pass-through debug camera positioning remains).
