#include "enemy.h"
#include "game.h"  // For shared variables like wallGrid
#include <math.h>
#include "config.h"

// Define the enemies array
Enemy enemies[MAX_ENEMIES];

// Initialize all enemies as inactive
void InitializeEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
}

// Spawn a new enemy at a given position and type
void SpawnEnemy(Vector2 position, int type) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].position = position;
            enemies[i].type = type;
            enemies[i].active = true;

            // Set properties based on type
            switch (type) {
                case 0:  // Basic enemy
                    enemies[i].speed = ENEMY_BASIC_SPEED;
                    enemies[i].health = ENEMY_BASIC_HEALTH;
                    enemies[i].color = RED;
                    break;
                case 1:  // Fast enemy
                    enemies[i].speed = 4.0f;
                    enemies[i].health = 1;
                    enemies[i].color = GREEN;
                    break;
                case 2:  // Tank enemy
                    enemies[i].speed = 1.0f;
                    enemies[i].health = 10;
                    enemies[i].color = BLUE;
                    break;
                default:
                    enemies[i].speed = 2.0f;
                    enemies[i].health = 3;
                    enemies[i].color = WHITE;
                    break;
            }
            break;  // Exit after spawning one enemy
        }
    }
}

// Update all active enemies
void UpdateEnemies(Vector2 playerPosition) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Move towards the player
            Vector2 direction = {
                playerPosition.x - enemies[i].position.x,
                playerPosition.y - enemies[i].position.y
            };

            // Normalize the direction vector
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
            }

            // Update position
            enemies[i].position.x += direction.x * enemies[i].speed;
            enemies[i].position.y += direction.y * enemies[i].speed;

            // Check for collisions with walls
            if (CheckCollision(wallGrid, enemies[i].position, TILE_SIZE, TILE_SIZE, TILE_SIZE)) {
                // Reverse direction if colliding with a wall
                enemies[i].position.x -= direction.x * enemies[i].speed;
                enemies[i].position.y -= direction.y * enemies[i].speed;
            }

            // Deactivate if health reaches 0
            if (enemies[i].health <= 0) {
                enemies[i].active = false;
            }
        }
    }
}

// Draw all active enemies
void DrawEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            DrawRectangleV(enemies[i].position, (Vector2){TILE_SIZE, TILE_SIZE}, enemies[i].color);
        }
    }
}

// Check collision between an enemy and a rectangle
bool CheckEnemyCollision(Enemy enemy, Vector2 position, int width, int height) {
    Rectangle enemyRect = {enemy.position.x, enemy.position.y, TILE_SIZE, TILE_SIZE};
    Rectangle projectileRect = {position.x, position.y, (float)width, (float)height};
    return CheckCollisionRecs(enemyRect, projectileRect);
}