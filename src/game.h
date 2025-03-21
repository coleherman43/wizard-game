#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "config.h"

// Shared variables
extern Vector2 mostRecentDirection;
extern int wallGrid[GRID_HEIGHT][GRID_WIDTH];

// Shared functions
bool CheckCollision(int wallGrid[GRID_HEIGHT][GRID_WIDTH], Vector2 position, int tileSize, int playerWidth, int playerHeight);
void DrawWalls(int wallGrid[GRID_HEIGHT][GRID_WIDTH], int tileSize);

#endif