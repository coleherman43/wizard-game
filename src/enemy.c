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

// enemy.c
WaveManager waveManager = {0};

void StartNewWave(WaveManager* manager, Vector2 playerPosition) {
    manager->currentWave++;
    manager->waveActive = true;
    
    // Calculate number of enemies
    int baseEnemies = BASE_ENEMIES_PER_WAVE * pow(WAVE_SCALE_FACTOR, manager->currentWave);
    manager->enemiesRemaining = baseEnemies;

    // Determine enemy types based on wave
    for (int i = 0; i < baseEnemies; i++) {
        EnemyType type = ENEMY_TYPE_BASIC;
        
        if (manager->currentWave >= 3 && GetRandomValue(0, 100) < 30) {
            type = ENEMY_TYPE_FAST;
        }
        if (manager->currentWave >= 5 && GetRandomValue(0, 100) < 15) {
            type = ENEMY_TYPE_TANK;
        }

        // Find spawn position away from player
        Vector2 spawnPos;
        do {
            spawnPos = (Vector2){
                playerPosition.x + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS),
                playerPosition.y + GetRandomValue(-SPAWN_RADIUS, SPAWN_RADIUS)
            };
        } while (Vector2Distance(spawnPos, playerPosition) < SPAWN_RADIUS/2);

        SpawnEnemy(spawnPos, type);
    }
}

void UpdateWaveSystem(WaveManager* manager, Vector2 playerPosition) {
    if (!manager->waveActive) {
        manager->waveTimer -= GetFrameTime();
        if (manager->waveTimer <= 0) {
            StartNewWave(manager, playerPosition);
        }
        return;
    }

    // Check if wave is complete
    int activeEnemies = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) activeEnemies++;
    }

    if (activeEnemies == 0 && manager->enemiesRemaining <= 0) {
        manager->waveActive = false;
        manager->waveTimer = WAVE_COOLDOWN;
        printf("Wave %d complete! Next wave in %.1f seconds\n", 
              manager->currentWave, WAVE_COOLDOWN);
    }
}
