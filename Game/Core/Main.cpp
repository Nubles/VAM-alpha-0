#include "../../Engine/Core/Window.h"
#include "../../Engine/Core/Input.h"
#include "../../Engine/Core/Timer.h"
#include "Game.h"
#include <iostream>

int main() {
    std::cout << "[Main] Launching Realmbound Wilds..." << std::endl;

    // Create 1280x720 window
    Engine::Window window(1280, 720, "Realmbound Wilds [Milestone 0: Project Setup]");

    if (!window.Init()) {
        std::cerr << "[Main] Failed to initialize window layer!" << std::endl;
        return -1;
    }

    // Initialize Static Input Tracker
    Engine::Input::Init(window.GetGLFWWindow());

    // Create Game Instance
    Game::Game game;
    if (!game.Init(&window)) {
        std::cerr << "[Main] Failed to initialize game layer!" << std::endl;
        return -1;
    }

    // High-resolution clock
    Engine::Timer timer;

    // Main loop
    while (!window.ShouldClose() && game.IsRunning()) {
        // Tick timer to calculate delta time
        timer.Tick();
        float dt = timer.GetDeltaTime();

        // 1. Poll window events (Keyboard/Mouse/Resize)
        window.PollEvents();

        // 2. Update input deltas (Mouse DX/DY)
        Engine::Input::Update();

        // 3. Update game simulation
        game.Update(dt);

        // 4. Render graphics & Dear ImGui HUD
        game.Render();

        // 5. Present frame to screen
        window.SwapBuffers();
    }

    std::cout << "[Main] Shutting down. Cleaning up resources..." << std::endl;
    
    // Cleanup game resources
    game.Shutdown();
    
    // Shutdown engine window context
    window.Shutdown();

    std::cout << "[Main] Game exited successfully." << std::endl;
    return 0;
}
