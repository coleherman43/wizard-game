#include <math.h>
#include "raylib.h"
#include "projectile.h"
#include <stdio.h>
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "config.h"

Vector2 mostRecentDirection = {1, 0};  // Default to right

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

// Function declarations
void DrawWalls(int wallGrid[GRID_HEIGHT][GRID_WIDTH], int tileSize);
bool CheckCollision(int wallGrid[GRID_HEIGHT][GRID_WIDTH], Vector2 position, int tileSize, int playerWidth, int playerHeight);
void HandleInput(Vector2 *direction, Player *player, Vector2 playerPosition, Vector2 *mostRecentDirection);
void UpdatePlayer(Vector2 *playerPosition, Vector2 direction, float playerSpeed, Player *player);
void UpdateEnemiesAndCheckCollisions(Vector2 playerPosition, Player *player);
void UpdateProjectilesAndHandleShooting(Vector2 playerPosition, Player *player);
void DrawGame(Texture2D playerTexture, Vector2 playerPosition, Player player);

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
    int gridX1 = (int)(position.x / tileSize);
    int gridY1 = (int)(position.y / tileSize);
    int gridX2 = (int)((position.x + playerWidth - 1) / tileSize);
    int gridY2 = (int)((position.y + playerHeight - 1) / tileSize);

    if (wallGrid[gridY1][gridX1] == 1 || wallGrid[gridY1][gridX2] == 1 ||
        wallGrid[gridY2][gridX1] == 1 || wallGrid[gridY2][gridX2] == 1) {
        return true;
    }
    return false;
}

// Handle player input
void HandleInput(Vector2 *direction, Player *player, Vector2 playerPosition, Vector2 *mostRecentDirection) {
    *direction = (Vector2){0, 0};

    if (IsKeyDown(KEY_RIGHT)) direction->x += 1;
    if (IsKeyDown(KEY_LEFT)) direction->x -= 1;
    if (IsKeyDown(KEY_DOWN)) direction->y += 1;
    if (IsKeyDown(KEY_UP)) direction->y -= 1;

    // Update mostRecentDirection if moving
    if (direction->x != 0 || direction->y != 0) {
        *mostRecentDirection = *direction;  // Update shooting direction
    }

    // Handle shooting with mostRecentDirection
    if (IsKeyPressed(KEY_SPACE)) {  // Fireball
        if (player->mana >= MANA_COST_FIREBALL) {
            ShootProjectile(playerPosition, *mostRecentDirection, 0); // Use dereferenced direction
            player->mana -= MANA_COST_FIREBALL;
        }
    }
    if (IsKeyPressed(KEY_L)) {  // Lightning
        if (player->mana >= MANA_COST_LIGHTNING) {
            ShootProjectile(playerPosition, *mostRecentDirection, 1);
            player->mana -= MANA_COST_LIGHTNING;
        }
    }
    if (IsKeyPressed(KEY_I)) {  // Ice
        if (player->mana >= MANA_COST_ICE) {
            ShootProjectile(playerPosition, *mostRecentDirection, 2);
            player->mana -= MANA_COST_ICE;
        }
    }
}

// Update player position and state
void UpdatePlayer(Vector2 *playerPosition, Vector2 direction, float playerSpeed, Player *player) {
    Vector2 newPosition = *playerPosition;
    newPosition.x += direction.x * playerSpeed;
    newPosition.y += direction.y * playerSpeed;

    // Check for collisions before updating the player's position
    if (!CheckCollision(wallGrid, newPosition, TILE_SIZE, TILE_SIZE, TILE_SIZE)) {
        *playerPosition = newPosition;
    }

    // Regenerate mana over time
    static float manaRegenTimer = 0.0f;
    manaRegenTimer += GetFrameTime();
    if (manaRegenTimer >= 1.0f) {
        player->mana += 1;
        if (player->mana > player->maxMana) player->mana = player->maxMana;
        manaRegenTimer = 0.0f;
    }
    CheckLevelUp(player);
}

// Update enemies and check for collisions with the player
void UpdateEnemiesAndCheckCollisions(Vector2 playerPosition, Player *player) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {

            // Guard to prevent dead enemies from interacting
            if (enemies[i].health <= 0) continue;

            // Check for collision with player
            bool playerCollision = CheckEnemyCollision(&enemies[i], playerPosition, TILE_SIZE, TILE_SIZE);
            if (playerCollision) {
                TakeDamage(player, 10);
                enemies[i].active = false;
                printf("Player collided with enemy %d! Player took 10 damage.\n", i);
            }

            // Check for collision with projectiles
            for (int j = 0; j < MAX_PROJECTILES; j++) {
                if (projectiles[j].active) {
                    // Get projectile collision size based on type
                    int width, height;
                    switch (projectiles[j].type) {
                        case 0: width = 20; height = 20; break; // Fireball
                        case 1: width = 20; height = 5;  break; // Lightning
                        case 2: width = 16; height = 16; break; // Ice
                        default: width = 10; height = 10; break;
                    }

                    // Check for collision
                    bool collision = CheckEnemyCollision(&enemies[i], projectiles[j].position, width, height);

                    // Debug output
                    printf("Enemy %d at (%.1f, %.1f) | Projectile %d at (%.1f, %.1f) | Collision: %d\n",
                           i, enemies[i].position.x, enemies[i].position.y,
                           j, projectiles[j].position.x, projectiles[j].position.y,
                           collision);
                    printf("Enemy Rect: (%.1f, %.1f, %d, %d) | Projectile Rect: (%.1f, %.1f, %d, %d)\n",
                           enemies[i].position.x - 10, enemies[i].position.y - 10, 20, 20,
                           projectiles[j].position.x, projectiles[j].position.y, width, height);

                    // Handle collision
                    if (collision) {
                        enemies[i].active = false;
                        projectiles[j].active = false;
                        player->xp += 10;  // Award XP
                        printf("Enemy %d defeated! Gained 10 XP. Current XP: %d/%d\n", i, player->xp, player->xpToNextLevel);
                        break;  // Exit the loop after handling the collision
                    }
                }
            }
        }
    }

    // Update enemy positions
    UpdateEnemies(playerPosition);
}

// Update projectiles and handle shooting
void UpdateProjectilesAndHandleShooting(Vector2 playerPosition, Player *player) {
    UpdateProjectiles();
}

// Draw the game
void DrawGame(Texture2D playerTexture, Vector2 playerPosition, Player player) {
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw walls
        DrawWalls(wallGrid, TILE_SIZE);

        // Draw player texture
        float scale = (float)TILE_SIZE / playerTexture.width;
        DrawTextureEx(playerTexture, playerPosition, 0.0f, scale, WHITE);

        // Draw projectiles and enemies
        DrawProjectiles();
        DrawEnemies();

        // Draw player bars
        DrawPlayerBars(player);
    EndDrawing();
}

// main.c
void DrawWaveInfo(WaveManager manager) {
    if (!manager.waveActive) {
        char waveText[50];
        snprintf(waveText, sizeof(waveText), "Next wave in: %.1f", manager.waveTimer);
        DrawText(waveText, 10, 50, 20, DARKGRAY);
    }
    
    char waveNumber[30];
    snprintf(waveNumber, sizeof(waveNumber), "Wave: %d", manager.currentWave);
    DrawText(waveNumber, 10, 10, 20, DARKGRAY);
}

int main() {
    // Initialize the window
    InitWindow(800, 600, "Wizard Game");
    SetTargetFPS(60);

    // Load the player texture
    Texture2D playerTexture = LoadTexture("assets/player.png");
    if (playerTexture.id == 0) {
        printf("Failed to load player texture. Check the file path.\n");
        CloseWindow();
        return 1;
    }

    // Initialize game objects
    InitializeEnemies();
    InitializeProjectiles();
    Player player;
    InitializePlayer(&player);

    // Player position and speed
    Vector2 playerPosition = {100, 100};
    float playerSpeed = 5.0f;

    // Initialize wave system
    StartNewWave(&waveManager, playerPosition); // Start first wave

    // Main game loop
    while (!WindowShouldClose()) {
        Vector2 direction = {0, 0};

        // Handle input
        HandleInput(&direction, &player, playerPosition, &mostRecentDirection);

        // Update player
        UpdatePlayer(&playerPosition, direction, playerSpeed, &player);

        // Update enemies and check collisions
        UpdateEnemiesAndCheckCollisions(playerPosition, &player);

        // Update projectiles
        UpdateProjectilesAndHandleShooting(playerPosition, &player);

        // Update wave system
        UpdateWaveSystem(&waveManager, playerPosition);

        // Update enemies and check collisions
        if (waveManager.waveActive) {
            UpdateEnemiesAndCheckCollisions(playerPosition, &player);
        }

        // Draw wave info
        DrawWaveInfo(waveManager);

        // Draw the game
        DrawGame(playerTexture, playerPosition, player);
    }

    // Cleanup
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}