#ifndef _ENEMY_H
#define _ENEMY_H

#include "../system/defines.h"
#include "../system/graphics.h"
#include "../audio/audio.h"
#include "player.h"

#define ENEMY_COUNT 8
#define ENEMY_WIDTH 2
#define ENEMY_HEIGHT 2

#define ENEMY_SOUTH_START_X 13
#define ENEMY_SOUTH_START_Y 13
#define ENEMY_WEST_START_X 15
#define ENEMY_WEST_START_Y 11
#define ENEMY_NORTH_START_X 17
#define ENEMY_NORTH_START_Y 13
#define ENEMY_EAST_START_X 15
#define ENEMY_EAST_START_Y 17

typedef enum {
    INVADER, SWEEPER, SPARX, NUCLEUS, ASTEROID
} enemytype;

typedef struct {
    u8 x, y, active, movements, animationTime, killTime;
    u16 score;
    direction dir;
    enemytype type;
} Enemy;

extern Enemy enemyPool[];
extern u8 enemyStartXCoords[];
extern u8 enemyStartYCoords[];

extern u8 enemyNextSpawnTime;

void EnemyInit(direction, enemytype);
void EnemyDeactivate(u8);
void EnemyUpdate(u8);
void EnemyKill(u8, u8);

#endif
