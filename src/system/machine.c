#include "machine.h"

u8 credits = 0;
machinemode machineMode = TITLE;
u32 hi = 10000;

u8 coinSwitchDown = false;
int inputs[] = { 0, 0 };

u8 coinsPerPlay = 1;
u8 controllers = 2;
u8 attractSound = true;
u8 livesPerPlay = 3;

void MachineLoadFromEEPROM();

void MachineSetup() {
    srand(GetTrueRandomSeed());

    SetTileTable(tiles);
    ClearVram();

    MachineLoadFromEEPROM();

    AudioInit();
}

void MachineLoadFromEEPROM() {
    struct EepromBlockStruct block;
    if(!EepromReadBlock(EEPROM_BLOCK, &block)) {
        coinsPerPlay = block.data[0];
        controllers = block.data[1];
        attractSound = block.data[2];
        livesPerPlay = block.data[3];
    }
}

void MachineCrossHatch() {
    u8 i = 90;
    Fill(0, 0, 32, 28, 252);
    while(i--) {
        WaitVsync(1);
    }

    i = 32;
    while(i--) {
        Fill(i, 0, 1, 28, 0);
        WaitVsync(1);
    }
}

void MachineInitHi() {
    SetTile(31, 14, 71);
    SetTile(31, 13, 70);
    PrintU32Vertical(30, 16, hi, 9999999);
}

u8 MachineInput() {
    // Read inputs (unrolled)
    inputs[0] = ReadJoypad(0);
    inputs[1] = ReadJoypad(1);

    // Stop here if test mode
    if(machineMode == TEST) {
        return true;
    }

    // Test switch
    if(inputs[0] & BTN_SR) {
        return false;
    }

    // Coin/Service
    if(inputs[0] & BTN_SL || inputs[1] & BTN_SL || inputs[1] & BTN_SR) {
        if(!coinSwitchDown) {
            if(credits < 9) {
                credits++;
                if(machineMode != GAME && coinsPerPlay) {
                    PrintU8Vertical(0, 18, credits);
                }
            }
            SFXPlay(PATCH_COIN_UP);
            coinSwitchDown = true;
        }
    } else if(coinSwitchDown) {
        coinSwitchDown = false;
    }

    return true;
}
