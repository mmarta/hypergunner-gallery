#include "test.h"

testoption selectedOption;
u8 buttonPressed = false;
u8 dirPressed = false;

void TestChangeOption();
void TestShowOption();

void TestInit() {
    machineMode = TEST;

    ClearVram();

    PrintVerticalRAM(29, 5, "HYPERGUNNER GALLERY");
    PrintVerticalRAM(28, 9, "TEST  MENU");

    PrintVerticalRAM(26, 3, "COINS PER PLAY");
    PrintVerticalRAM(25, 3, "PANEL CONTROLS");
    PrintVerticalRAM(24, 3, "ATTRACT SOUND");
    PrintVerticalRAM(23, 3, "LIVES PER PLAY");
    PrintVerticalRAM(22, 3, "SAVE AND EXIT");

    PrintVerticalRAM(20, 4, "USE P1 STICK TO MOVE");
    PrintVerticalRAM(19, 3, "USE P1 START TO UPDATE");

    PrintVerticalRAM(16, 9, "INPUT TEST");
    PrintVerticalRAM(14, 9, "P1      P2");
    PrintVerticalRAM(13, 1, "LEFT");
    PrintVerticalRAM(12, 1, "UP");
    PrintVerticalRAM(11, 1, "RIGHT");
    PrintVerticalRAM(10, 1, "DOWN");
    PrintVerticalRAM(9, 1, "FIRE");
    PrintVerticalRAM(8, 1, "COIN");
    PrintVerticalRAM(7, 1, "START");

    selectedOption = OPTION_COINS;
    while(selectedOption <= OPTION_EXIT) {
        TestShowOption();
        selectedOption++;
    }
    selectedOption = OPTION_COINS;
    PrintVerticalRAM(26 - selectedOption, 1, ">");
}

void TestSaveToEEPROM() {
    struct EepromBlockStruct block;
    block.id = EEPROM_BLOCK;
    block.data[0] = coinsPerPlay;
    block.data[1] = controllers;
    block.data[2] = attractSound;
    block.data[3] = livesPerPlay;
    EepromWriteBlock(&block);
}

void TestChangeOption() {
    switch(selectedOption) {
        case OPTION_COINS:
            coinsPerPlay++;
            if(coinsPerPlay == 5) {
                coinsPerPlay = 0;
            }
            break;
        case OPTION_CONTROLS:
            controllers = controllers == 2 ? 1 : 2;
            break;
        case OPTION_ATTRACT_SOUND:
            attractSound = !attractSound;
            break;
        case OPTION_LIVES:
            livesPerPlay++;
            if(livesPerPlay == 6) {
                livesPerPlay = 2;
            }
            break;
        default:
            // Save to eeprom
            TestSaveToEEPROM();
            machineMode = TITLE;
    }

    if(selectedOption != OPTION_EXIT) {
        TestShowOption();
    }
}

void TestShowOption() {
    switch(selectedOption) {
        case OPTION_COINS:
            if(!coinsPerPlay) {
                PrintVerticalRAM(26, 18, "FREE PLAY");
            } else {
                Fill(26, 18, 1, 9, 0);
                PrintU8Vertical(26, 26, coinsPerPlay);
            }
            break;
        case OPTION_CONTROLS:
            if(controllers == 2) {
                PrintVerticalRAM(25, 21, "  DUAL");
            } else {
                PrintVerticalRAM(25, 21, "SINGLE");
            }
            break;
        case OPTION_ATTRACT_SOUND:
            if(attractSound) {
                PrintVerticalRAM(24, 24, " ON");
            } else {
                PrintVerticalRAM(24, 24, "OFF");
            }
            break;
        case OPTION_LIVES:
            PrintU8Vertical(23, 26, livesPerPlay);
            break;
        default:
            return;
    }
}

void TestUpdate() {
    // Change option / Show button pressed
    if(inputs[0] & BTN_START) {
        if(!buttonPressed) {
            TestChangeOption();
            buttonPressed = true;
        }
    } else if(buttonPressed) {
        buttonPressed = false;
    }

    // Next option / Show direction pressed
    if(inputs[0] & BTN_DOWN) {
        if(!dirPressed) {
            SetTile(26 - selectedOption, 1, 0);
            selectedOption++;
            if(selectedOption > OPTION_EXIT) {
                selectedOption = 0;
            }
            PrintVerticalRAM(26 - selectedOption, 1, ">");
            dirPressed = true;
        }
    } else if(inputs[0] & BTN_UP) {
        if(!dirPressed) {
            SetTile(26 - selectedOption, 1, 0);
            selectedOption--;
            if(selectedOption > OPTION_EXIT) {
                selectedOption = OPTION_EXIT;
            }
            PrintVerticalRAM(26 - selectedOption, 1, ">");
            dirPressed = true;
        }
    } else if(dirPressed) {
        dirPressed = false;
    }

    // Input tests
    SetTile(13, 10, inputs[0] & BTN_LEFT ? 1 : 0);
    SetTile(13, 18, inputs[1] & BTN_LEFT ? 1 : 0);
    SetTile(12, 10, inputs[0] & BTN_UP ? 1 : 0);
    SetTile(12, 18, inputs[1] & BTN_UP ? 1 : 0);
    SetTile(11, 10, inputs[0] & BTN_RIGHT ? 1 : 0);
    SetTile(11, 18, inputs[1] & BTN_RIGHT ? 1 : 0);
    SetTile(10, 10, inputs[0] & BTN_DOWN ? 1 : 0);
    SetTile(10, 18, inputs[1] & BTN_DOWN ? 1 : 0);
    SetTile(9, 10, inputs[0] & BTN_A ? 1 : 0);
    SetTile(9, 18, inputs[1] & BTN_A ? 1 : 0);
    SetTile(8, 10, inputs[1] & BTN_SL ? 1 : 0);
    SetTile(8, 18, inputs[1] & BTN_SR ? 1 : 0);
    SetTile(7, 10, inputs[0] & BTN_START ? 1 : 0);
    SetTile(7, 18, inputs[0] & BTN_SELECT ? 1 : 0);
}
