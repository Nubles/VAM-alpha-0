# ROADMAP: Realmbound Wilds

This roadmap outlines the milestones required to build the initial playable vertical slice using a custom C++ engine. Each milestone contains low-level engine tasks (if required) and high-level gameplay tasks.

---

## Milestone 0: Project Setup
*Goal: Establish the windowing, rendering context, build system, and third-party dependencies.*
- [ ] **Engine Setup**: Create CMake project file (`CMakeLists.txt`) linking GLFW, GLAD, GLM, ImGui, STB, and JSON.
- [ ] **Window & Input**: Initialize GLFW window and establish basic input polling (Keyboard, Mouse).
- [ ] **Renderer Setup**: Write basic vertex and fragment shaders. Set up a simple render loop drawing a colored 3D cube.
- [ ] **Debug Integration**: Integrate Dear ImGui and implement a basic debug panel displaying FPS and frame time.

## Milestone 1: Player Movement & Camera
*Goal: Implement third-person camera controls, physics-based/kinematic movement, and interaction traces.*
- [ ] **Engine: Camera Class**: Implement a perspective camera system with mouse-look (pitch, yaw) and orbital positioning relative to a target.
- [ ] **Gameplay: Player Entity**: Create the `Player` class containing position, rotation, velocity, health, and stamina.
- [ ] **Gameplay: Movement & Jump**: Implement W/A/S/D movement relative to camera orientation and basic jumping with gravity.
- [ ] **Engine: Raycasting/Interaction**: Implement a forward raycast trace from the player/camera to detect object intersections for interactable elements.

## Milestone 2: Inventory & Item Data
*Goal: Build the item database framework and implement a working inventory/hotbar UI.*
- [ ] **Gameplay: Item Database**: Set up `items.json` parsing. Initialize the internal C++ dictionary of item definitions.
- [ ] **Gameplay: Inventory Structs**: Implement `ItemStack` and `Inventory` classes with helper functions for adding, removing, and checking items.
- [ ] **UI: Inventory Grid**: Create a debug inventory HUD overlay in ImGui to inspect items, equipment slots, and the hotbar.
- [ ] **Gameplay: Item Hotbar & Equipping**: Implement hotbar selection (keys 1-8) and trigger basic equipping actions (attaching weapon/tool visual to player hand).

## Milestone 3: Resource Gathering
*Goal: Spawn interactable nodes and implement harvesting mechanics and tools.*
- [ ] **Gameplay: Resource Node Entity**: Create a `ResourceNode` class representing trees, rocks, and plants.
- [ ] **Engine: Debug Rendering**: Implement a line-drawing system to render bounding boxes around resource nodes.
- [ ] **Gameplay: Harvesting Interaction**: Bind interaction controls so that hitting a tree with a Stone Axe drops wood items into the inventory, depleting the node.
- [ ] **Gameplay: Node Respawn / Deletion**: Implement node destruction, particle/sound placeholders, and respawn clocks.

## Milestone 4: Crafting System
*Goal: Build a recipe processor to allow players to construct new tools and equipment.*
- [ ] **Gameplay: Recipe Database**: Parse `recipes.json` config containing material costs and station requirements.
- [ ] **UI: Crafting Panel**: Create a workbench GUI in ImGui displaying available recipes, ingredients owned/needed, and a "Craft" button.
- [ ] **Gameplay: Crafting Action**: Implement recipe verification, ingredient consumption, and output item production.

## Milestone 5: Building Placement
*Goal: Implement a grid-based or free-form building system for placing basic structures.*
- [ ] **Engine: Ghost Renderer**: Support rendering a semi-transparent mesh for placement preview.
- [ ] **Gameplay: Building Pieces**: Define structural pieces (Floor, Wall, Roof) with recipe ingredient costs.
- [ ] **Gameplay: Placement Logic**: Implement raycasting to position structural pieces, rotation handling via mouse wheel, and snapping/collision checks.
- [ ] **Gameplay: Construction**: Finalize piece placement, consume resources from inventory, and register the piece as a permanent world entity.

## Milestone 6: Combat & Enemy AI
*Goal: Create basic combat mechanics and standard patrolling enemy AI.*
- [ ] **Gameplay: Enemy Entity**: Implement the `Enemy` class with simple state machine: Idle, Patrol, Chase, Attack, Return Home.
- [ ] **Gameplay: Patrol & Chase**: Implement basic pathfinding or steer-to-target chasing toward the player position.
- [ ] **Gameplay: Combat Hits**: Implement melee attacks for both player and enemy. Perform collision checks (bounding box overlaps).
- [ ] **Gameplay: Health & Damage**: Apply damage values, compute armor mitigation, handle player/enemy death, and drop loot on enemy death.

## Milestone 7: First Biome Prototype
*Goal: Implement a basic visual region with themed assets and lighting representing the first biome.*
- [ ] **Engine: Skybox & Lighting**: Create a directional light source representing the sun. Implement a rotating cycle with ambient color shifts.
- [ ] **Engine: Terrain/Environment Grid**: Build a simple flat grid or noise-based terrain mesh displaying grass/rock textures.
- [ ] **Gameplay: Biome Definition**: Load biome-specific settings (ambient light colors, enemy spawners, resource density).

## Milestone 8: Monolith Encounter (Mini-Boss/Shrine)
*Goal: Build the vertical slice completion objective and encounter mechanics.*
- [ ] **Gameplay: Monolith Shrine**: Build a shrine entity requiring activation (e.g., placing specific resources or interacting).
- [ ] **Gameplay: Elite Enemy Spawn**: Trigger a custom event on activation that spawns a larger, stronger "Guardian" mini-boss.
- [ ] **Gameplay: Realm Stabilization**: On guardian defeat, trigger a visual/atmospheric shift (e.g., clearing fog, changing sky colors) to denote region stabilization.

## Milestone 9: Save & Load System
*Goal: Implement persistent state serialization using JSON files.*
- [ ] **Gameplay: Serialization**: Write functions to serialize player state (health, position, inventory) and world state (built structures, harvested nodes) to JSON.
- [ ] **Gameplay: Deserialization**: Implement file reading to reconstruct the player and world state at startup.
- [ ] **UI: Save/Load Buttons**: Add save and load triggers to the ImGui debug overlay.

## Milestone 10: Vertical Slice Polish
*Goal: Resolve performance bottlenecks, polish UI, and clean code boundaries.*
- [ ] **Engine: Code Review**: Ensure strict separation of Engine and Game layers. Remove any hardcoded cross-dependencies.
- [ ] **Engine: Asset Loading**: Optimize mesh/texture loading and cache resources to avoid runtime hitching.
- [ ] **Gameplay: Audio / Visual Cues**: Add simple particle effect templates (for harvesting and combat hits) and simple audio placeholders.
- [ ] **Gameplay: Verification Walkthrough**: Complete a full playthrough of the vertical slice from spawn, to crafting, building, and defeating the mini-boss.
