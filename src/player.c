#include "player.h"
#include <stdio.h>
#include "config.h"

// Initialize player with default values
void InitializePlayer(Player *player) {
    player->position = (Vector2){100, 100};  // Default position
    player->health = PLAYER_DEFAULT_MAX_HEALTH;
    player->maxHealth = PLAYER_DEFAULT_MAX_HEALTH;
    player->mana = PLAYER_DEFAULT_MAX_MANA;
    player->maxMana = PLAYER_DEFAULT_MAX_MANA;
    player->level = PLAYER_DEFAULT_START_LEVEL;
    player->xp = PLAYER_DEFAULT_START_XP;
    player->xpToNextLevel = PLAYER_DEFAULT_XP_TO_NEXT_LEVEL;  // XP required to level up
    player->speed = PLAYER_DEFAULT_SPEED;
}

// Draw health and mana bars
void DrawPlayerBars(Player player) {
    int barWidth = 200;
    int barHeight = 20;
    int padding = 10;

    // Draw health bar
    DrawRectangle(padding, padding, barWidth, barHeight, RED);
    DrawRectangle(padding, padding, (int)(barWidth * ((float)player.health / player.maxHealth)), barHeight, GREEN);
    DrawText("Health", padding, padding - 20, 20, BLACK);

    // Draw mana bar
    DrawRectangle(padding, padding + barHeight + padding, barWidth, barHeight, BLUE);
    DrawRectangle(padding, padding + barHeight + padding, (int)(barWidth * ((float)player.mana / player.maxMana)), barHeight, SKYBLUE);
    DrawText("Mana", padding, padding + barHeight + padding - 20, 20, BLACK);

    // Draw level progress bar
    DrawRectangle(padding, padding + 2 * (barHeight + padding), barWidth, barHeight, GRAY);  // Background
    DrawRectangle(padding, padding + 2 * (barHeight + padding), (int)(barWidth * ((float)player.xp / player.xpToNextLevel)), barHeight, PURPLE);  // Progress
    DrawText("Level Progress", padding, padding + 2 * (barHeight + padding) - 20, 20, BLACK);

    // Draw level and XP text
    DrawText(TextFormat("Level: %d", player.level), padding, padding + 3 * (barHeight + padding), 20, BLACK);
    DrawText(TextFormat("XP: %d/%d", player.xp, player.xpToNextLevel), padding, padding + 4 * (barHeight + padding), 20, BLACK);
}

// Reduce player health
void TakeDamage(Player *player, int damage) {
    player->health -= damage;
    if (player->health < 0) {
        player->health = 0;
    }
    printf("Player took %d damage! Health: %d\n", damage, player->health);
}

// Increase player XP and handle level-ups
void GainXP(Player *player, int xp) {
    player->xp += xp;
    if (player->xp >= player->xpToNextLevel) {
        player->level++;
        player->xp -= player->xpToNextLevel;
        player->xpToNextLevel = (int)(player->xpToNextLevel * 1.5);  // Increase XP required for next level
        player->maxHealth += 20;  // Increase max health on level-up
        player->health = player->maxHealth;  // Heal player to full
        player->maxMana += 10;  // Increase max mana on level-up
        player->mana = player->maxMana;  // Restore mana to full
    }
}

void DeductMana(Player *player, int amount) {
    player->mana -= amount;
    if (player->mana < 0) {
        player->mana = 0;  // Ensure mana doesn't go below 0
    }
    printf("Deducted %d mana. Current mana: %d\n", amount, player->mana);  // Debug output
}

void CheckLevelUp(Player *player) {
    while (player->xp >= player->xpToNextLevel) {
        player->level++;  // Increase level
        player->xp -= player->xpToNextLevel;  // Deduct XP for the next level
        player->xpToNextLevel = (int)(player->xpToNextLevel * 1.5);  // Increase XP required for the next level

        // Apply level-up bonuses
        player->maxHealth += 20;  // Increase max health
        player->health = player->maxHealth;  // Heal player to full
        player->maxMana += 10;  // Increase max mana
        player->mana = player->maxMana;  // Restore mana to full

        printf("Level up! You are now level %d\n", player->level);  // Debug output
    }
}