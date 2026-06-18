# Combat & Enemy AI System

This document outlines the combat mechanics, enemy AI behavior states, health impact damage factors, and debug interface configured for Milestone 7.

## Combat Mechanics

* **Hit Check**: Triggered by clicking Left Mouse Button when looking at an active enemy within `3.0` units range (while NOT in Build Mode).
* **Player Damage**: Applies `20.0` damage per hit. Enemy vanishes or registers dead on depletion.
* **Loot Drop**: Defeating the enemy drops Stone (+3 quantity) and Raw Food (+1 quantity) into the player's inventory slots.

## Enemy AI patroller behavior

The enemy AI tracks states:
1. **Patrol State**: Moves slowly (`1.5 * 0.6` speed) around its spawn origin location between random generated offsets (4.0 units max radius), pausing for 3.0 seconds at each point.
2. **Chase State**: Enters chase when player is within `6.0` units distance. Moves directly toward the player.
3. **Attack State**: Enters attack when player is within `1.5` units. Deducts `10.0` health points from the player at a cooldown rate of `2.0` seconds.

## Intentional Constraints (Not Implemented)

* Threat ranges UI or sound effect cues.
* Weapon scaling (attack damage is constant, weapon items in inventory do not modify stats yet).
* Respawn rules (dead sentinels remain dead).
* Pathfinding collision avoiding (sentinels pass through walls/stones to target the player).
