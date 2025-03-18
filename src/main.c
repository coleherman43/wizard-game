#include "raylib.h"

int main() {
    // Initialize the window
    InitWindow(800, 600, "My Raylib Game");

    // Set the target frames per second
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        // Update logic here

        // Draw here
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello, Raylib!", 190, 200, 50, DARKGRAY);
        EndDrawing();
    }

    // Close the window
    CloseWindow();
    return 0;
}