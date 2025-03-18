#include "raylib.h"
#include <stdio.h>

// Wall grid dimensions
#define GRID_WIDTH 16
#define GRID_HEIGHT 12
#define TILE_SIZE 50

// Wall grid (1 = wall, 0 = empty)
int wallGrid[GRID_HEIGHT][GRID_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Draw walls
void DrawWalls(int wallGrid[GRID_HEIGHT][GRID_WIDTH], int tileSize) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (wallGrid[y][x] == 1) {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, GRAY);
            }
        }
    }
    // FOR DEBUGGING: show rectangles
    // for (int y = 0; y < GRID_HEIGHT; y++) {
    //     for (int x = 0; x < GRID_WIDTH; x++) {
    //         DrawText(TextFormat("%d", wallGrid[y][x]), x * TILE_SIZE + 10, y * TILE_SIZE + 10, 20, BLACK);
    //     }
    // }
}

// Check for collisions
bool CheckCollision(int wallGrid[GRID_HEIGHT][GRID_WIDTH], Vector2 position, int tileSize, int playerWidth, int playerHeight) {
    // Check all four edges of the player's bounding box
    int gridX1 = (int)(position.x / tileSize);
    int gridY1 = (int)(position.y / tileSize);

    int gridX2 = (int)((position.x + playerWidth - 1) / tileSize);  // Right edge
    int gridY2 = (int)((position.y + playerHeight - 1) / tileSize); // Bottom edge

    // Check if any edge is inside a wall
    if (wallGrid[gridY1][gridX1] == 1 ||  // Top-left corner
        wallGrid[gridY1][gridX2] == 1 ||  // Top-right corner
        wallGrid[gridY2][gridX1] == 1 ||  // Bottom-left corner
        wallGrid[gridY2][gridX2] == 1) {  // Bottom-right corner
        return true;  // Collision detected
    }
    return false;  // No collision
}

int main() {
    // Initialize the window
    InitWindow(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE, "Wizard Game");

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
    float scale = (float)TILE_SIZE / playerTexture.width;

    // Player position and size
    Vector2 playerPosition = {50, 50};  // Updated starting position
    float playerSpeed = 5.0f;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update logic
        Vector2 newPosition = playerPosition;

        if (IsKeyDown(KEY_RIGHT)) newPosition.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT)) newPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN)) newPosition.y += playerSpeed;
        if (IsKeyDown(KEY_UP)) newPosition.y -= playerSpeed;

        // Check for collisions
        if (!CheckCollision(wallGrid, newPosition, TILE_SIZE, TILE_SIZE, TILE_SIZE)) {
            playerPosition = newPosition;
        }

        // Draw here
        BeginDrawing();
            ClearBackground(RAYWHITE);  // Clear the background to white

            // Draw walls
            DrawWalls(wallGrid, TILE_SIZE);

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