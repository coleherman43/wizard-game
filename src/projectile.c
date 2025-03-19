#include "projectile.h"

Projectile projectiles[MAX_PROJECTILES];

void InitializeProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
}

void ShootProjectile(Vector2 position, Vector2 direction, int type) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].position = position;
            projectiles[i].direction = direction;
            projectiles[i].speed = 10.0f;
            projectiles[i].active = true;
            projectiles[i].type = type;

            switch (type) {
                case 0:  // Fireball
                    projectiles[i].color = RED;
                    break;
                case 1:  // Lightning
                    projectiles[i].color = YELLOW;
                    break;
                case 2:  // Ice
                    projectiles[i].color = BLUE;
                    break;
                default:
                    projectiles[i].color = WHITE;
                    break;
            }
            break;
        }
    }
}

void UpdateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            projectiles[i].position.x += projectiles[i].direction.x * projectiles[i].speed;
            projectiles[i].position.y += projectiles[i].direction.y * projectiles[i].speed;

            if (projectiles[i].position.x < 0 || projectiles[i].position.x > GetScreenWidth() ||
                projectiles[i].position.y < 0 || projectiles[i].position.y > GetScreenHeight()) {
                projectiles[i].active = false;
            }
        }
    }
}

void DrawProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            DrawCircleV(projectiles[i].position, 10, projectiles[i].color);
        }
    }
}