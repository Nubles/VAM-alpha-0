# Basic 3D Sandbox

This document outlines the architecture, controls, and rendering decisions for the **Milestone 1: Basic 3D Sandbox** prototype.

## Rendering Backend Decision
The project uses **OpenGL 3.3 Core Profile** (linked via **GLFW** and **GLAD**) as its rendering backend.
- **Why OpenGL 3.3?**: OpenGL 3.3 Core is cross-platform, light on compile times, highly stable, and requires zero heavy SDK installations. It provides a simple programmable pipeline (Vertex/Fragment shaders) ideal for building procedural and debug graphics.
- **Unlit Shader Support**: The fragment shader (`cube.frag`) includes an `unlit` toggle, enabling rendering of lines/grids with flat constant colors alongside standard Phong-shaded 3D geometry.

---

## Architecture

```
Engine/                     Game/
├── Math/                   ├── Core/
│   ├── MathTypes.h         │   ├── Game.h / Game.cpp (Scene list & loop)
│   └── Transform.h         │   ├── SceneObject.h (Object definitions)
│                           │   └── Main.cpp (Entry point)
└── Renderer/               └── Assets/
    ├── Camera.h / .cpp         └── Shaders/
    └── Shader.h / .cpp             ├── cube.vert
                                    └── cube.frag
```

- **MathTypes**: Declares namespaces aliases `Engine::Vec2`, `Engine::Vec3`, and `Engine::Mat4` mapped to the underlying GLM implementation.
- **Transform**: Encapsulates Translation, Euler Rotation (pitch, yaw, roll in degrees), and Scale, outputting a model transform matrix via `GetModelMatrix()`.
- **SceneObject**: Game-specific structure binding a human-readable Name, an `Engine::Transform`, a primitive mesh Type (Cube, Grid), and a base Color.

---

## Controls & Interaction

| Key / Mouse Action | Control Action |
|---|---|
| **W, A, S, D** | Translate camera forward, left, backward, right |
| **Mouse Motion** | Rotate camera (orbital Pitch and Yaw look) |
| **Left Shift / Right Shift** | Hold to accelerate camera movement by **3x** |
| **Left Mouse Click** | Lock cursor to capture mouse-look inputs |
| **Left ALT** | Release cursor to interact with the Dear ImGui panel |
| **Escape** | Close window and exit cleanly |

---

## Run and Verification

### 1. Compile & Build
From the workspace root, configure and build the Debug executable using CMake:
```powershell
# Configure (if clean or CMakeLists.txt edited)
& "C:\Users\alexa\Pictures\CUI\.venv\Scripts\cmake.exe" -B build -S . "-DCMAKE_POLICY_VERSION_MINIMUM=3.5"

# Build Debug target
& "C:\Users\alexa\Pictures\CUI\.venv\Scripts\cmake.exe" --build build --config Debug
```

### 2. Run Sandbox
Launch the compiled binary:
```powershell
& build/bin/Debug/RealmboundWilds.exe
```

---

## Intentionally Not Implemented Yet
To maintain scope and modularity, the following are omitted from Milestone 1:
1. **Model Loading**: Primitives (Cubes and Grids) are generated procedurally at startup. OBJ/FBX parsing is scheduled for later milestones.
2. **Heightmap Terrain**: The ground is represented by a flat debug line grid. Noise-based heightmap terrain meshes will be built in the Biome milestones.
3. **Player Physics / Collision**: Camera movement is free-flying and clip-less, enabling quick debugging. Character physics, gravity, and ground bounds will be added in the player controller milestone.
4. **Gameplay Systems**: Inventory databases, crafting tables, items, enemies, and world saving are mock-designed but have no active code in this sandbox.
