// #include "enemy.h"
#include "game.h"  // For shared variables like wallGrid
#include <math.h>
#include "config.h"
#include "raymath.h"
#include "enemy.h"


Enemy enemies[MAX_ENEMIES];

void InitializeEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
}

void SpawnEnemy(Vector2 position, EnemyType type) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].position = position;
            enemies[i].type = type;
            enemies[i].health = (type == ENEMY_TYPE_BASIC) ? 3 :
                                (type == ENEMY_TYPE_FAST) ? 2 : 5;
            enemies[i].speed = (type == ENEMY_TYPE_BASIC) ? 0.5f :
                               (type == ENEMY_TYPE_FAST) ? 4.0f : 1.5f;
            enemies[i].color = (type == ENEMY_TYPE_BASIC) ? RED :
                               (type == ENEMY_TYPE_FAST) ? BLUE : GREEN;
            enemies[i].active = true;
            break;
        }
    }
}

void UpdateEnemies(Vector2 playerPosition) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Calculate direction toward the player
            Vector2 direction = Vector2Subtract(playerPosition, enemies[i].position);
            direction = Vector2Normalize(direction);
            enemies[i].direction = direction;

            // Move the enemy
            Vector2 velocity = Vector2Scale(enemies[i].direction, enemies[i].speed);
            enemies[i].position = Vector2Add(enemies[i].position, velocity);

            // Check for death
            if (enemies[i].health <= 0) {
                enemies[i].active = false;
            }
        }
    }
}

void DrawEnemies(void) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Draw a square instead of a circle
            DrawRectangle(
                enemies[i].position.x - 10,  // Center X (20x20 square)
                enemies[i].position.y - 10,  // Center Y
                20,  // Width
                20,  // Height
                enemies[i].color
            );
        }
    }
}

bool CheckEnemyCollision(const Enemy *enemy, Vector2 position, int width, int height) {
    // Enemy collision rectangle (centered)
    Rectangle enemyRect = {
        enemy->position.x - 10,  // Center X
        enemy->position.y - 10,  // Center Y
        20, 20
    };

    // Projectile collision rectangle (centered)
    Rectangle projectileRect = {
        position.x - width/2,  // Center X
        position.y - height/2, // Center Y
        width,
        height
    };

    return CheckCollisionRecs(enemyRect, projectileRect);
}