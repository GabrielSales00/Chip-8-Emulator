/*
Author: Gabriel Sales

State University of Campinas (Unicamp)

Chip 8 emulator
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "include/chip8window.h"
#include "include/chip8.h"

int main(int argc, char * argv[]) {
    char const * address = argv[1];

    if (address == NULL) { //will load tetris by default if no game is specified.
        printf("Please use the rom_path as the arg.\n");
        return -1;
    }

    Screen * screen = newScreen("Chip 8 emulator by Gabriel Sales", SCREEN_HEIGHT * 10, SCREEN_WIDTH * 10, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    chip8 * cp8 = newChip8();

    sub_00E0(cp8); //subroutine the initializes the display. Should I put it in the constructor?


    loadFile(address, cp8);

    int pitch = sizeof(cp8->video[0]) * SCREEN_WIDTH;

    bool quit = false;


    int cycleDelay = 4;

    struct timespec lastCycleTime, currentTime;

    clock_gettime(CLOCK_MONOTONIC, &lastCycleTime);
    


    while (!quit) {
        quit = takeInput(cp8->keypad);
        
        clock_gettime(CLOCK_MONOTONIC, &currentTime);
        float difference = (currentTime.tv_sec - lastCycleTime.tv_sec) * 1000.0f + (currentTime.tv_nsec - lastCycleTime.tv_nsec) / 1.0e6f;


        if (difference > cycleDelay) {

            lastCycleTime = currentTime;

            execution_cycle(cp8);

            update(screen, cp8->video, pitch); 
        }

    }

    closeScreen(screen);
    closeChip8(cp8);

}