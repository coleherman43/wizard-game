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
#define ENEMY_BASE_HEALTH 3           // Base stats for enemy types
#define ENEMY_BASE_SPEED 0.5f
#define ENEMY_BASE_XP 10

// Projectile stats
#define MAX_PROJECTILES 100
#define MANA_COST_FIREBALL 10
#define MANA_COST_LIGHTNING 20
#define MANA_COST_ICE 15

// Wave system
#define STARTING_WAVE 1
#define BASE_ENEMIES_PER_WAVE 5
#define WAVE_SCALE_FACTOR 1.2    // 20% more enemies per wave
#define SPAWN_RADIUS 150.0f      // Spawn enemies at least this far from player
#define WAVE_COOLDOWN 3.0f       // Seconds between waves

#endif