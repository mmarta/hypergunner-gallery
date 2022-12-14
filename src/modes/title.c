#include "title.h"

u16 titleTime = 0;
u8 prevCredits = 0;

u8 wireX;
u8 wireY;

const char barkyStr1[] PROGMEM = "DESIGNED & PROGRAMMED IN C";
const char barkyStr2[] PROGMEM = "BY MARC MARTA";

void TitleBeginDemo();
void TitleDemoUpdate();
void TitleDemoClean();

void TitleStart() {
    if(!coinsPerPlay) {
        PrintVerticalRAM(0, 18, "FREE  PLAY");
    } else {
        PrintVerticalRAM(0, 9, "CREDITS");
        PrintU8Vertical(0, 18, credits);
    }
    titleTime = 0;
    activePlayer = 0;
}

void TitleBeginDemo() {
    u8 i;
    CLEAR_TITLE();
    BackgroundStart();
    PlayerStart(0, 1);
    PlayerTurnReset();

    PrintVerticalRAM(17, 9, "GAME");
    PrintVerticalRAM(17, 15, "OVER");

    i = ENEMY_COUNT;
    while(i--) {
        EnemyDeactivate(i);
    }

    i = LASER_COUNT;
    while(i--) {
        LaserDeactivate(i);
    }
}

void TitleDemoClean() {
    u8 i;
    players[activePlayer].active = false;

    i = ENEMY_COUNT;
    while(i--) {
        EnemyDeactivate(i);
    }

    i = LASER_COUNT;
    while(i--) {
        LaserDeactivate(i);
    }
}

void TitleUpdate() {
    u8 modTime = titleTime % 60;
    u8 modTimeStarfighter = titleTime % 2;

    // Standard Title Screen
    if(titleTime < 540) {
        // Text sequence
        switch(titleTime) {
            case 0:
                // Clear space & scores aside from Hi
                CLEAR_TITLE();
                Fill(30, 0, 2, 7, 0);
                Fill(30, 21, 2, 7, 0);
                break;
            case 60:
                PrintVerticalRAM(27, 12, "PLAY");
                break;
            case 120:
                PrintVerticalRAM(25, 5, "HYPERGUNNER GALLERY");
                break;
            case 180:
                PrintVerticalPROGMEM(23, 1, barkyStr1);
                PrintVerticalPROGMEM(22, 8, barkyStr2);
                PrintVerticalRAM(6, 3, "@2020, 2022 RED BALLTOP");
                break;
            case 240:
                players[activePlayer].animationTime = 0;
                players[activePlayer].killTime = 0;
                players[activePlayer].hookEnabled = false;
                players[activePlayer].x = 11;
                players[activePlayer].y = 13;
                players[activePlayer].dir = SOUTH;
                break;
            case 248:
                players[activePlayer].dir = WEST;
                break;
            case 256:
                players[activePlayer].dir = NORTH;
                break;
            case 264:
                players[activePlayer].dir = EAST;
                break;
            case 272:
                players[activePlayer].dir = SOUTH;
                break;
            case 300:
                wireX = players[activePlayer].x + 2;
                wireY = players[activePlayer].y + 2;
                DrawMap(wireX, wireY, mapHookNW);
                if(attractSound) {
                    SFXPlay(PATCH_PLAYER_LAUNCH_HOOK);
                }
            case 301:
            case 302:
            case 303:
            case 304:
            case 305:
                DrawMap(wireX, wireY, mapWireA);
                wireX++;
                wireY++;
                DrawMap(wireX, wireY, mapHookNE);
                break;
            case 329:
                while(wireX > players[activePlayer].x) {
                    SetTile(wireX, wireY, 0);
                    wireX--;
                    wireY--;
                }
            case 330:
                wireX = players[activePlayer].x + 2;
                wireY = players[activePlayer].y - 1;
                DrawMap(wireX, wireY, mapHookNE);
                if(attractSound) {
                    SFXPlay(PATCH_PLAYER_LAUNCH_HOOK);
                }
            case 331:
            case 332:
            case 333:
            case 334:
            case 335:
                DrawMap(wireX, wireY, mapWireB);
                wireX++;
                wireY--;
                DrawMap(wireX, wireY, mapHookNW);
                break;
            case 359:
                while(wireX > players[activePlayer].x) {
                    SetTile(wireX, wireY, 0);
                    wireX--;
                    wireY++;
                }
        }

        // Hypergunner Starfighter sprite
        if(titleTime >= 240) {
            if(!modTimeStarfighter) {
                PlayerDrawTiles();
            }
        }

        players[activePlayer].animationTime = players[activePlayer].animationTime == 7 ? 0 : players[activePlayer].animationTime + 1;
    } else if(titleTime == 540) { // Begin demo mode
        TitleBeginDemo();
    } else {
        TitleDemoUpdate();
    }

    // Insert coin? Push 1P/2P start?
    if(!modTime || (modTime < 30 && prevCredits != credits)) {
        if(!coinsPerPlay) {
            PrintVerticalRAM(1, 4, "PRESS 1P OR 2P START");
        } else if(!(credits / coinsPerPlay)) {
            PrintVerticalRAM(1, 8, "INSERT  COIN");
        } else if(credits / coinsPerPlay == 1) {
            PrintVerticalRAM(1, 7, "PRESS 1P START");
        } else {
            PrintVerticalRAM(1, 4, "PRESS 1P OR 2P START");
        }
    } else if(modTime == 30) {
        Fill(1, 0, 1, 28, 0);
    }

    prevCredits = credits;
    MachineInput();

    // Start
    // Free Play?
    if(!coinsPerPlay) {
        if(inputs[0] & BTN_START) {
            GameStart(1);
            return;
        } else if(inputs[0] & BTN_SELECT) {
            GameStart(2);
            return;
        }
    } else if(credits) {
        if(inputs[0] & BTN_START && (credits / coinsPerPlay >= 1)) {
            credits -= coinsPerPlay;
            GameStart(1);
            return;
        } else if(inputs[0] & BTN_SELECT && (credits / coinsPerPlay >= 2)) {
            credits -= (coinsPerPlay << 1);
            GameStart(2);
            return;
        }
    }


    titleTime++;
    if(titleTime == 1120) {
        TitleDemoClean();
        titleTime = 0;
    }
}

void TitleDemoUpdate() {
    switch(titleTime) {
        case 600:
            EnemyInit(WEST, INVADER);
            break;
        case 620:
            PlayerLaunchHook(WEST);
            break;
        case 660:
            PlayerFire();
            break;
        case 680:
            EnemyInit(SOUTH, SWEEPER);
            break;
        case 690:
            PlayerLaunchHook(SOUTH);
            break;
        case 730:
            PlayerFire();
            break;
        case 770:
            EnemyInit(NORTH, INVADER);
            break;
        case 800:
            PlayerLaunchHook(EAST);
            break;
        case 830:
            EnemyInit(WEST, ASTEROID);
            EnemyInit(NORTH, INVADER);
            PlayerLaunchHook(NORTH);
            break;
        case 860:
            EnemyInit(EAST, SPARX);
            PlayerFire();
            break;
        case 890:
            PlayerFire();
            break;
        case 930:
            PlayerLaunchHook(EAST);
            break;
        case 1015:
            PlayerFire();
            EnemyInit(SOUTH, SPARX);
            break;
        case 1030:
            PlayerLaunchHook(SOUTH);
            EnemyInit(WEST, NUCLEUS);
            break;
        case 1060:
            PlayerFire(); // Will kill player
            EnemyInit(NORTH, ASTEROID);
            break;
    }

    GameUpdate();
}
