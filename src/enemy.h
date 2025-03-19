#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "config.h"

typedef struct {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    int type;  // 0 = basic, 1 = fast, 2 = tank, etc.
    bool active;
    Color color;  // Visual representation
} Enemy;

// Declare the enemies array as extern
extern Enemy enemies[MAX_ENEMIES];

// Function declarations
void InitializeEnemies();
void SpawnEnemy(Vector2 position, int type);
void UpdateEnemies(Vector2 playerPosition);
void DrawEnemies();
bool CheckEnemyCollision(Enemy enemy, Vector2 position, int width, int height);

#endif