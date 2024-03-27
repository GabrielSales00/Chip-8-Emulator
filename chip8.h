/*
Author: Gabriel Sales

State University of Campinas (Unicamp)

Chip 8 emulator
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //para geração de números aleatórios
#include <string.h>



#define START_ADDRESS 0x200 //Instruções de software começam a partir do 0x200 pois 0x000 ~ 0xFFF são reservadas
#define FONTARR_SIZE 80 // 16 chars de 5 bytes
#define FONTARR_START_ADDRESS 0x50 //endereço de onde é esperada a presença do primeiro caractere
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MAX_GAME_SIZE (0x1000 - 0x200)


//data structure representing the computer system
typedef struct chip8 {
    uint8_t registers[16];
    uint8_t memory[4096];
    uint16_t index;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t keypad[16];
    uint32_t video[2048]; //64 * 32 = 2048
    uint16_t opcode;
} chip8;

void closeChip8(chip8* cp8) {
    free(cp8);
}

void loadFile(const char* fileAddress, chip8 * cp8) {

    FILE * game;
    long length;

    game = fopen(fileAddress, "rb");

    if (game == NULL) {
        printf("Opening of file %s failed.\n", fileAddress);
        exit(1);
    }
    else {
        printf("Loading of file %s successful. \n", fileAddress);

        printf("Loading game into memory, now...\n");
        //loading the file into memory
        fread(&cp8->memory[START_ADDRESS], 1, MAX_GAME_SIZE, game);

        fclose(game);
    }
}


/*
    Isso vai permitir carregar as fontes dos caracteres de 0~9 e de A~F.
    Roms esperam esses 16 caracteres, de 8 bits cada (1 byte), em endereços específicos.
*/
void loadBasicFontsIntoMemory (chip8 * cp8) {

    //vetor contendo caracteres a serem desenhado nas telas
    uint8_t basicFontsArr[FONTARR_SIZE] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7    
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (unsigned aux = 0; aux < FONTARR_SIZE; aux++) {
        cp8 -> memory[FONTARR_START_ADDRESS + aux] = basicFontsArr[aux];
    }
}

uint8_t generateRandomNum() {
    srand(time(NULL));
    return (rand() % (255 - 0 + 1) + 0);
}

chip8 * newChip8() {
    chip8 * cp8;
    cp8 = (chip8 *) malloc(sizeof(chip8));

    cp8->index = 0;
	cp8->sp = 0;
	cp8->soundTimer = 0;
	cp8->delayTimer = 0;
	cp8->opcode = 0;	
    cp8 -> pc = START_ADDRESS;

    for(int i = 0; i < 16; i++) {
        cp8->registers[i] = 0;
    }

    loadBasicFontsIntoMemory(cp8);

    return cp8;
}


