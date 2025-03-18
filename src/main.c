#include "raylib.h"
#include <stdio.h>

int main() {
    // Initialize the window
    InitWindow(800, 600, "Wizard Game");

    // Set the target frames per second
    SetTargetFPS(60);

    // Load the player texture
    Texture2D playerTexture = LoadTexture("assets/player.png");
    if (playerTexture.id == 0) {
        printf("Failed to load texture. Check the file path.\n");
        CloseWindow();
        return 1;
    }

    // Calculate the scale to fit the player within a 50x50 grid square
    float scale = 50.0f / playerTexture.width;

    // Player position and size
    Vector2 playerPosition = {400, 300};
    float playerSpeed = 5.0f;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update logic
        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < 800 - 50) playerPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN) && playerPosition.y < 600 - 50) playerPosition.y += playerSpeed;
        if (IsKeyDown(KEY_UP) && playerPosition.y > 0) playerPosition.y -= playerSpeed;

        // Draw here
        BeginDrawing();
            ClearBackground(RAYWHITE);  // Clear the background to white

            // Draw a grid background
            for (int i = 0; i < 800; i += 50) {
                DrawLine(i, 0, i, 600, LIGHTGRAY);
            }
            for (int j = 0; j < 600; j += 50) {
                DrawLine(0, j, 800, j, LIGHTGRAY);
            }

            // Draw the player texture (scaled)
            DrawTextureEx(playerTexture, playerPosition, 0.0f, scale, WHITE);
        EndDrawing();
    }

    // Unload the player texture
    UnloadTexture(playerTexture);

    // Close the window
    CloseWindow();
    return 0;
}