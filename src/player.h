#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int score;
    int level;
    int xp;
    int xpToNextLevel;
} Player;

// Function declarations
void InitializePlayer(Player *player);
void DrawPlayerBars(Player player);
void TakeDamage(Player *player, int damage);
void GainXP(Player *player, int xp);
void CheckLevelUp(Player *player);

#endif