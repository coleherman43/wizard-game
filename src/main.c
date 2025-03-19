#include <math.h>
#include "raylib.h"
#include "projectile.h"
#include <stdio.h>
#include "game.h"
#include "enemy.h"
#include "player.h"

#define GRID_WIDTH 16
#define GRID_HEIGHT 12
#define TILE_SIZE 50

Vector2 mostRecentDirection = {1, 0};  // Default to right
int playerHealth = 100;  // Player starts with 100 health
int playerMana = 100;    // Player starts with 100 mana

// Wall grid (1 = wall, 0 = empty)
int wallGrid[GRID_HEIGHT][GRID_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
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
    InitWindow(800, 600, "Wizard Game");

    // Set the target frames per second
    SetTargetFPS(60);

    // Load the player texture
    Texture2D playerTexture = LoadTexture("assets/player.png");  // Replace with your texture path
    if (playerTexture.id == 0) {
        printf("Failed to load player texture. Check the file path.\n");
        CloseWindow();
        return 1;
    }

    // Initialize projectiles
    InitializeEnemies();
    InitializeProjectiles();
    // Initialize Player
    Player player;
    InitializePlayer(&player);
    

    // Player position and size
    Vector2 playerPosition = {100, 100};
    float playerSpeed = 5.0f;
    SpawnEnemy((Vector2){250, 250}, 0);  // Basic enemy

    // Main game loop
    while (!WindowShouldClose()) {
        // Update logic
        Vector2 newPosition = playerPosition;

        Vector2 direction = {0, 0};  // Combined direction vector

        

        if (IsKeyDown(KEY_RIGHT)) direction.x += 1;
        if (IsKeyDown(KEY_LEFT)) direction.x -= 1;
        if (IsKeyDown(KEY_DOWN)) direction.y += 1;
        if (IsKeyDown(KEY_UP)) direction.y -= 1;

        // Normalize the combined direction vector
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        // Check for player-enemy collisions
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                bool collision = CheckEnemyCollision(enemies[i], playerPosition, TILE_SIZE, TILE_SIZE);
                if (collision) {
                    TakeDamage(&player, 10);
                    enemies[i].active = false;
                }
            }
        }

        // Regenerate mana over time
        static float manaRegenTimer = 0.0f;
        manaRegenTimer += GetFrameTime();
        if (manaRegenTimer >= 1.0f) {
            player.mana += 1;
            if (player.mana > player.maxMana) player.mana = player.maxMana;
            manaRegenTimer = 0.0f;
        }

        // Update the player's position
        newPosition.x += direction.x * playerSpeed;
        newPosition.y += direction.y * playerSpeed;

        // Update the most recent direction
        if (direction.x != 0 || direction.y != 0) {
            mostRecentDirection = direction;
        }

        // Check for collisions before updating the player's position
        if (!CheckCollision(wallGrid, newPosition, TILE_SIZE, TILE_SIZE, TILE_SIZE)) {
            playerPosition = newPosition;
        }

        // Update projectiles
        UpdateProjectiles();
        UpdateEnemies(playerPosition);

        // Handle shooting
        if (IsKeyPressed(KEY_SPACE)) {
            // Fireball
            ShootProjectile(playerPosition, mostRecentDirection, 0);
        }
        if (IsKeyPressed(KEY_L)) {
            // Lightning
            ShootProjectile(playerPosition, mostRecentDirection, 1);
        }
        if (IsKeyPressed(KEY_I)) {
            // Ice
            ShootProjectile(playerPosition, mostRecentDirection, 2);
        }

        // Draw here
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw walls
            DrawWalls(wallGrid, TILE_SIZE);

            // Draw player texture
            float scale = (float)TILE_SIZE / playerTexture.width;  // Scale to fit grid
            DrawTextureEx(playerTexture, playerPosition, 0.0f, scale, WHITE);

            
            DrawProjectiles(); // Draw projectiles
            DrawEnemies();
            DrawPlayerBars(player);  // Draw health, mana, score, level, and XP
        EndDrawing();
    }

    // Unload the player texture
    UnloadTexture(playerTexture);

    // Close the window
    CloseWindow();
    return 0;
}