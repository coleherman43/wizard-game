#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "raylib.h"

#define MAX_PROJECTILES 100

typedef struct {
    Vector2 position;
    Vector2 direction;
    float speed;
    Color color;
    int type;
    bool active;
} Projectile;

// Declare the projectiles array as extern
extern Projectile projectiles[MAX_PROJECTILES];

// Function declarations
void InitializeProjectiles();
void ShootProjectile(Vector2 position, Vector2 direction, int type);
void UpdateProjectiles();
void DrawProjectiles();

#endif