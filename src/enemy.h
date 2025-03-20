#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "config.h"

// Use an enum for enemy types instead of raw integers
typedef enum {
    ENEMY_TYPE_BASIC = 0,
    ENEMY_TYPE_FAST,
    ENEMY_TYPE_TANK,
    NUM_ENEMY_TYPES // Keeps track of the number of enemy types
} EnemyType;

typedef struct {
    Vector2 position;
    Vector2 direction;
    float speed;
    int health;
    EnemyType type;  // Use the enum instead of int
    bool active;
    Color color;  // Visual representation
} Enemy;

// Declare the enemies array as extern
extern Enemy enemies[MAX_ENEMIES];

// Function declarations
void InitializeEnemies(void);
void SpawnEnemy(Vector2 position, EnemyType type);
void UpdateEnemies(Vector2 playerPosition);
void DrawEnemies(void);
bool CheckEnemyCollision(const Enemy *enemy, Vector2 position, int width, int height);

#endif