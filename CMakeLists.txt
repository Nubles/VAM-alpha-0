cmake_minimum_required(VERSION 3.15)
project(RealmboundWilds CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure output directories for compiled binary and assets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# FetchContent module for dependency management
include(FetchContent)

# -----------------------------------------------------------------------------
# 1. Fetch & Configure GLFW
# -----------------------------------------------------------------------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    GIT_SHALLOW ON
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# -----------------------------------------------------------------------------
# 2. Fetch & Configure GLM (Mathematics)
# -----------------------------------------------------------------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(glm)

# -----------------------------------------------------------------------------
# 3. Fetch & Configure nlohmann_json (Serialization)
# -----------------------------------------------------------------------------
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW ON
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(nlohmann_json)

# -----------------------------------------------------------------------------
# 4. Configure GLAD (Local OpenGL Loader)
# -----------------------------------------------------------------------------
add_library(glad STATIC
    Engine/ThirdParty/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    Engine/ThirdParty/glad/include
)

# -----------------------------------------------------------------------------
# 5. Fetch & Configure Dear ImGui (Debug HUD)
# -----------------------------------------------------------------------------
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.5
    GIT_SHALLOW ON
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    
    # Compile Dear ImGui + Glfw/OpenGL3 backends as a static library
    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC 
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(imgui PUBLIC glfw glad)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif()

# -----------------------------------------------------------------------------
# 6. Realmbound Wilds Main Application Target
# -----------------------------------------------------------------------------

# Source files list (Engine and Game logic)
set(SOURCE_FILES
    Engine/Core/Window.h
    Engine/Core/Window.cpp
    Engine/Core/Input.h
    Engine/Core/Input.cpp
    Engine/Core/Timer.h
    Engine/Core/Timer.cpp
    Engine/Renderer/Shader.h
    Engine/Renderer/Shader.cpp
    Engine/Renderer/Camera.h
    Engine/Renderer/Camera.cpp
    Engine/Math/MathTypes.h
    Engine/Math/Transform.h
    
    Game/Core/Game.h
    Game/Core/Game.cpp
    Game/Core/SceneObject.h
    Game/Core/Main.cpp
)

# Executable target
add_executable(RealmboundWilds ${SOURCE_FILES})

# Include directories
target_include_directories(RealmboundWilds PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/_deps/glm-src
)

# Link runtime libraries
target_link_libraries(RealmboundWilds PRIVATE
    glfw
    glad
    glm
    imgui
    nlohmann_json::nlohmann_json
)

# Copy Assets folder to the build/bin directory so paths resolve correctly
add_custom_command(TARGET RealmboundWilds POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets
    $<TARGET_FILE_DIR:RealmboundWilds>/Assets
)

# Enable testing framework
enable_testing()

# Define EngineCoreTests target
add_executable(EngineCoreTests Tests/EngineCoreTests.cpp)
target_include_directories(EngineCoreTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}/_deps/glm-src
)
target_link_libraries(EngineCoreTests PRIVATE glm)

# Add CTest check
add_test(NAME EngineCoreTests COMMAND EngineCoreTests)

