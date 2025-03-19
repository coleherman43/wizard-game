#include "projectile.h"
#include "game.h"
Projectile projectiles[MAX_PROJECTILES];

void InitializeProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = false;
    }
}

void ShootProjectile(Vector2 position, Vector2 direction, int type) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            // Initialize the projectile
            projectiles[i].position = position;
            projectiles[i].direction = direction;
            projectiles[i].active = true;
            projectiles[i].type = type;

            // Set properties based on the type
            switch (type) {
                case 0:  // Fireball
                    projectiles[i].speed = 10.0f;
                    projectiles[i].color = RED;
                    break;
                case 1:  // Lightning
                    projectiles[i].speed = 15.0f;  // Faster than fireball
                    projectiles[i].color = YELLOW;
                    break;
                case 2:  // Ice
                    projectiles[i].speed = 7.0f;   // Slower than fireball
                    projectiles[i].color = BLUE;
                    break;
                default:
                    projectiles[i].speed = 10.0f;
                    projectiles[i].color = WHITE;
                    break;
            }
            break;  // Exit after activating one projectile
        }
    }
}

void UpdateProjectiles() {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            // Move the projectile
            projectiles[i].position.x += projectiles[i].direction.x * projectiles[i].speed;
            projectiles[i].position.y += projectiles[i].direction.y * projectiles[i].speed;

            // Check for collisions with walls
            if (CheckCollision(wallGrid, projectiles[i].position, TILE_SIZE, 10, 10)) {  // 10x10 is the projectile size
                projectiles[i].active = false;  // Deactivate the projectile
            }

            // Deactivate if off-screen
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
            switch (projectiles[i].type) {
                case 0:  // Fireball
                    DrawCircleV(projectiles[i].position, 10, projectiles[i].color);
                    break;
                case 1:  // Lightning
                    DrawRectangleV(projectiles[i].position, (Vector2){20, 5}, projectiles[i].color);  // Rectangular lightning bolt
                    break;
                case 2:  // Ice
                    DrawCircleV(projectiles[i].position, 8, projectiles[i].color);  // Smaller ice projectile
                    break;
                default:
                    DrawCircleV(projectiles[i].position, 10, WHITE);  // Default projectile
                    break;
            }
        }
    }
}