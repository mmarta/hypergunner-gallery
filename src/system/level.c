#include "level.h"

const char readyStr[] PROGMEM = "READY!";
const char gameOverStr[] PROGMEM = "GAME  OVER";
const char player1Str[] PROGMEM = "PLAYER 1";
const char player2Str[] PROGMEM = "PLAYER 2";

const Level levels[] = {
    {
        .invaderFactor = 9, .sweeperFactor = 0, .sparxFactor = 0, .nucleusFactor = 0,
        .asteroidFactor = 2, .bonus = 1000, .randomFactor = 50, .minGenTime = 20
    },
    {
        .invaderFactor = 6, .sweeperFactor = 4, .sparxFactor = 0, .nucleusFactor = 0,
        .asteroidFactor = 3, .bonus = 2000, .randomFactor = 40, .minGenTime = 20
    },
    {
        .invaderFactor = 3, .sweeperFactor = 7, .sparxFactor = 0, .nucleusFactor = 0,
        .asteroidFactor = 3, .bonus = 3000, .randomFactor = 40, .minGenTime = 15
    },
    {
        .invaderFactor = 2, .sweeperFactor = 7, .sparxFactor = 2, .nucleusFactor = 0,
        .asteroidFactor = 5, .bonus = 4000, .randomFactor = 40, .minGenTime = 15
    },
    {
        .invaderFactor = 0, .sweeperFactor = 7, .sparxFactor = 2, .nucleusFactor = 2,
        .asteroidFactor = 6, .bonus = 5000, .randomFactor = 40, .minGenTime = 12
    },
    {
        .invaderFactor = 2, .sweeperFactor = 7, .sparxFactor = 2, .nucleusFactor = 4,
        .asteroidFactor = 6, .bonus = 6000, .randomFactor = 35, .minGenTime = 12
    },
    {
        .invaderFactor = 2, .sweeperFactor = 9, .sparxFactor = 2, .nucleusFactor = 6,
        .asteroidFactor = 6, .bonus = 7000, .randomFactor = 30, .minGenTime = 12
    },
    {
        .invaderFactor = 3, .sweeperFactor = 9, .sparxFactor = 2, .nucleusFactor = 7,
        .asteroidFactor = 7, .bonus = 8000, .randomFactor = 30, .minGenTime = 10
    },
    {
        .invaderFactor = 0, .sweeperFactor = 4, .sparxFactor = 2, .nucleusFactor = 9,
        .asteroidFactor = 7, .bonus = 9000, .randomFactor = 25, .minGenTime = 10
    },
    {
        .invaderFactor = 0, .sweeperFactor = 4, .sparxFactor = 2, .nucleusFactor = 9,
        .asteroidFactor = 7, .bonus = 10000, .randomFactor = 25, .minGenTime = 5
    },
    {
        .invaderFactor = 3, .sweeperFactor = 9, .sparxFactor = 2, .nucleusFactor = 7,
        .asteroidFactor = 7, .bonus = 10000, .randomFactor = 20, .minGenTime = 5
    },
    {
        .invaderFactor = 0, .sweeperFactor = 4, .sparxFactor = 2, .nucleusFactor = 9,
        .asteroidFactor = 7, .bonus = 10000, .randomFactor = 20, .minGenTime = 2
    }
};

u8 levelTime = 30;
u8 levelTick = 60;

u8 bonusTextTime = 0;
u16 bonusTextNum = 0;

void LevelIncrease();
void LevelDisplayGameOver();

void LevelStart() {
    levelTime = 30;
    PrintVerticalRAM(1, 21, "TIME");
    PrintU8Vertical(1, 27, levelTime);
    PrintU8Vertical(1, 7, players[activePlayer].level + 1);
    PrintVerticalRAM(1, 0, "STAGE");
}

void LevelIncrease() {
    u8 i = ENEMY_COUNT;
    u8 levelToGrab = players[activePlayer].level >= 12
        ? (players[activePlayer].level % 3) + 9
        : players[activePlayer].level;

    // Eliminate all enemies & award player enemy points & bonus
    while(i--) {
        if(enemyPool[i].active && !enemyPool[i].killTime) {
            // Set NUCLEUS to small & kill, deactivate asteroids, kill all others
            if(enemyPool[i].type == NUCLEUS && enemyPool[i].animationTime < 60) {
                enemyPool[i].animationTime = 60;
                EnemyKill(i, false);
            } else if(enemyPool[i].type == ASTEROID) {
                EnemyDeactivate(i);
            } else {
                EnemyKill(i, false);
            }
            PlayerAddScoreDelta(enemyPool[i].score);
        } else if(enemyPool[i].active && enemyPool[i].type == NUCLEUS && enemyPool[i].killTime && enemyPool[i].animationTime < 60) {
            // Nucleus in transition to smaller? Set to small, kill, award player points
            enemyPool[i].animationTime = 60;
            EnemyKill(i, false);
            PlayerAddScoreDelta(enemyPool[i].score);
        }
    }
    PlayerAddScoreDelta(levels[levelToGrab].bonus);
    bonusTextNum = levels[levelToGrab].bonus;

    // Increase the actual level
    if(players[activePlayer].level < 255) {
        players[activePlayer].level++;
    }
    bonusTextTime = 160;
    if(attractSound || machineMode == GAME) {
        SFXPlay(PATCH_NEXT_LEVEL);
    }
    LevelStart();
}

void LevelUpdate() {
    if(!levelTick) {
        levelTime--;
        PrintU8Vertical(1, 27, levelTime);
        if(!levelTime) {
            LevelIncrease();
        }
        levelTick = 60;
    } else {
        levelTick--;
    }

    if(!bonusTextTime) {
        PrintVerticalRAM(0, 8, "            ");
    } else {
        u8 modTime = bonusTextTime % 32;
        if(modTime == 0) {
            PrintVerticalRAM(0, 8, "BONUS");
            PrintU32Vertical(0, 19, bonusTextNum, 99999);
        } else if(modTime == 16) {
            PrintVerticalRAM(0, 8, "            ");
        }
        bonusTextTime--;
    }
}

u8 LevelPlayerDeath() {
    u8 i, nextPlayer;

    if(!players[activePlayer].active) {
        LevelDisplayGameOver();
    }

    // Move to next player?
    nextPlayer = activePlayer + 1;
    nextPlayer = nextPlayer % playersInGame;
    if(players[nextPlayer].active) {
        activePlayer = nextPlayer;
    }

    // No more players? End here
    if(!players[activePlayer].active) {
        return false;
    }

    // Clear the screen and write
    LevelStart();
    PlayerDrawLives(activePlayer);
    LevelDisplayReady(90);

    // Deactivate all enemies
    i = ENEMY_COUNT;
    while(i--) {
        EnemyDeactivate(i);
    }

    return true;
}

void LevelDisplayReady(u8 timer) {
    CLEAR_LEVEL();
    PrintVerticalPROGMEM(17, 11, readyStr);
    PrintVerticalPROGMEM(15, 10, activePlayer ? player2Str : player1Str);
    while(timer--) {
        MachineInput();
        WaitVsync(1);
    }

    CLEAR_LEVEL();
}

void LevelDisplayGameOver() {
    u8 timer = 180;
    CLEAR_LEVEL();
    MusicPlayPatch(MUSIC_GAME_OVER);
    PrintVerticalPROGMEM(17, 9, gameOverStr);
    PrintVerticalPROGMEM(15, 10, activePlayer ? player2Str : player1Str);
    while(timer--) {
        MachineInput();
        WaitVsync(1);
    }
    CLEAR_LEVEL();
}
