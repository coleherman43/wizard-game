#ifndef CONFIG_H
#define CONFIG_H

// Grid dimensions
#define GRID_WIDTH 16
#define GRID_HEIGHT 12
#define TILE_SIZE 50

// Player stats
#define PLAYER_DEFAULT_MAX_HEALTH 100
#define PLAYER_DEFAULT_MAX_MANA 100
#define PLAYER_DEFAULT_START_LEVEL 1
#define PLAYER_DEFAULT_START_XP 0
#define PLAYER_DEFAULT_XP_TO_NEXT_LEVEL 100
#define PLAYER_DEFAULT_SPEED 5.0f

// Enemy stats
// Enemy configuration
#define MAX_ENEMIES 100               // Max simultaneous enemies
#define INITIAL_WAVE_SIZE 1           // Enemies in first wave
#define WAVE_GROWTH_RATE 2            // e.g., 2x enemies per wave
#define ENEMY_BASE_HEALTH 3           // Base stats for enemy types
#define ENEMY_BASE_SPEED 0.5f
#define ENEMY_BASE_XP 10

// Projectile stats
#define MAX_PROJECTILES 100
#define MANA_COST_FIREBALL 10
#define MANA_COST_LIGHTNING 20
#define MANA_COST_ICE 15

#endif