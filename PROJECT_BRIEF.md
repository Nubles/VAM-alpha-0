# PROJECT BRIEF: Realmbound Wilds

## Game Vision
**Realmbound Wilds** is a solo-developer-friendly survival crafting game set in a mysterious, unstable fantasy wilderness. The player explores regions, gathers resources, builds outposts, and fights guardian bosses to survive and stabilize the environment. The atmosphere is mythic, ancient, and slightly magical, with a visual identity focused on weather-beaten hand-built structures and rune-carved wilderness.

## Unique Twist: Realm Bleed & Stability
The world is comprised of unstable, shifting "realms" that bleed into one another. 
- **Realm Fractures**: Cracks in the world that lead to temporary pocket dimensions/zones containing rare materials or challenges.
- **Outpost Stabilization**: By defeating Biome Guardians or activating stabilizing monoliths, the player permanently anchors and claims a region, clearing the corruption and enabling safe building, farming, or ritual activities.
- **Realm Energy**: Claimed areas generate or harness magical realm energy, which is used to power ancient gateways or upgrade crafting stations.

## Core Pillars
1. **Explore**: Traverse a semi-procedural, region-based wilderness filled with ancient ruins, caves, monoliths, and realm fractures.
2. **Survive**: Manage preparation (buffs, food, warmth, comfort, gear) rather than constant punishment. A prepared explorer thrives; an unprepared one struggles.
3. **Craft**: Harvest resources to craft survival gear, equipment, weapons, and specialized exploration tools.
4. **Build**: Construct bases, defensive walls, portals, and processing outposts. Custom C++ engine rendering will support lightweight grid/free-form building.
5. **Progress**: Materials, recipes, and biome access are unlocked dynamically by defeating regional guardians.
6. **Prepare for Danger**: Bosses and environmental events (realm storms, raids) reward correct weapon choice, resistances, and status preparations.
7. **Expand Modularly**: Designed from the ground up to allow rapid additions of new biomes, items, enemies, and structural components.

## What We Are NOT Copying (Distinct from Valheim)
- **No Norse Mythology Copying**: Avoid Odin, Valhalla, Meadows/Black Forest naming, and Viking aesthetics. Instead, use a unique mythic-fantasy setting with distinct lore.
- **No Low-Poly/Pixel-Art Texture Clone**: Use a clean, stylized but grounded material look (e.g., hand-painted or stylized PBR with atmospheric lighting) that looks modern and artistic, utilizing custom shader effects.
- **No Copy-Pasted Progression**: Gate progression through realm stabilization, shifting pocket dimensions, and energy management, rather than simple metal-tier upgrades.
- **No Copy-Pasted UI**: Establish a distinct, clean, rune-inspired minimal UI design.

## Initial Vertical Slice Target
The vertical slice will serve as the first playable milestone to validate the custom C++ engine and core gameplay:
- **One Handcrafted Level**: A small sandbox containing a starting zone, a resource gathering area, a simple enemy camp, and one shrine objective (the Stabilizing Monolith).
- **Player Controller**: Third-person movement, camera rotation, jumping, basic attacks, health, and stamina.
- **Basic Gathering**: Chop-able primitive trees, mineable stones, pickable plants.
- **Inventory & Hotbar**: Fixed-slot inventory structure storing item stacks, types, and stats.
- **Basic Crafting**: Workbench-equivalent UI to craft basic tools (stone axe, wooden club).
- **Simple Building**: Placement of basic structures (floor, wall, roof) using preview/ghost rendering.
- **Basic Enemy AI**: A "Realm-Rot" scavenger that patrols, detects the player, chases, attacks, and retreats.
- **Day/Night Cycle**: Basic direction light rotation and sky color interpolation representing time.
- **Monolith Encounter (Mini-Boss/Shrine)**: Activating a shrine spawns a tougher elite enemy; defeating it stabilizes the region, ending the vertical slice.
- **Debug Menu**: Visual overlay (ImGui) to toggle god mode, spawn items, change time of day, and view performance metrics.

## Long-Term Features
- Fully procedural world generation using heightmaps and noise.
- Co-op multiplayer architecture (using socket-level networking or a lightweight library like ENet/Steamworks).
- Realm fracture pocket dimensions (instanced levels).
- Advanced building physics or structural integrity systems.
- Agriculture, animal taming, and automated realm energy harvesters.
