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
#include "include/chip8.h"



//these are implementations of the original subroutines performed by the chip-8 system
void sub_default(chip8 *cp8) { //does nothing, useful in case of an unimplemented subroutine

}

void sub_00E0(chip8 *cp8) { //CLS - limpar o display
    memset(cp8->video, 0, sizeof(cp8->video));
}

void sub_00EE(chip8 *cp8) { //RET - return
    (cp8->sp)--;
    cp8->pc = cp8->stack[cp8->sp]; //notação feia
}

void sub_1nnn(chip8 *cp8) { //JP addr - pular para um endereço
    uint16_t address = cp8->opcode & 0x0FFFu;
    cp8->pc = address;
}

void sub_2nnn(chip8 *cp8) { //CALL addr - chamar subrotina / função de end
    uint16_t address = cp8->opcode & 0x0FFFu;
    cp8->stack[cp8->sp] = cp8->pc;
    (cp8->sp)++;
    cp8->pc = address;
}

void sub_3xkk(chip8 *cp8) { //Pula próxima instrução se o Vx do registro for igual a kk
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = cp8->opcode & 0x00FFu;

    if(cp8->registers[Vx] == byte) {
        cp8->pc += 2;
    }
}

void sub_4xkk(chip8 *cp8) { // SNE Vx, byte pular próxima instrução se vx != kk
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = cp8->opcode & 0x00FFu;

    if(cp8->registers[Vx] != byte) {
        cp8->pc += 2;
    }
}

void sub_5xy0(chip8 *cp8) { //SE Vx, Vy
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;

    if (cp8->registers[Vx] == cp8->registers[Vy]){
        cp8->pc += 2;
    }
}

void sub_6xkk(chip8 *cp8) { //LD  Vx, byte
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = cp8->opcode & 0x00FFu;

    cp8->registers[Vx] = byte; 
}

void sub_7xkk(chip8 *cp8) { //ADD Vx, byte
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = cp8->opcode & 0x00FFu;

    cp8->registers[Vx] += byte; 
}

void sub_8xy0(chip8 *cp8) { //LD  Vx, VY
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;


    cp8->registers[Vx] = cp8->registers[Vy]; 
}

void sub_8xy1(chip8 *cp8) { //OR  Vx, Vy
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;


    cp8->registers[Vx] != cp8->registers[Vy]; 
}

void sub_8xy2(chip8 *cp8) { //AND  Vx, vy
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;


    cp8->registers[Vx] &= cp8->registers[Vy]; 
}

void sub_8xy3(chip8 *cp8) { //XOR  Vx, Vy
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;


    cp8->registers[Vx] ^= cp8->registers[Vy]; 
}

void sub_8xy4(chip8 *cp8) { //ADD Vx, Vy 
//(se o resultado for maior que 8 bits, o flag é 1)
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;


    uint16_t sum = cp8->registers[Vx] + cp8->registers[Vy];

    if(sum > 255U) {
        cp8->registers[0xF] = 1;
    }
    else {
        cp8->registers[0xF] = 0;
    }

    cp8->registers[Vx] = sum & 0xFFu;
}


void sub_8xy5(chip8 *cp8) { //SUB Vx, Vy 
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;

    if(cp8->registers[Vx] > cp8 -> registers[Vy]) {
        cp8->registers[0xF] = 1;
    }
    else {
        cp8->registers[0xF] = 0;
    }

    cp8->registers[Vx] -= cp8 -> registers[Vy];
}

void sub_8xy6(chip8 *cp8) { //SHR VX
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

	cp8->registers[0xF] = (cp8->registers[Vx] & 0x1u);

	cp8->registers[Vx] >>= 1;
}


void sub_8xy7(chip8 *cp8) { //SUBN Vx, Vy 
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;

    if(cp8->registers[Vy] > cp8 -> registers[Vx]) {
        cp8->registers[0xF] = 1;
    }
    else {
        cp8->registers[0xF] = 0;
    }

    cp8->registers[Vx] = cp8 -> registers[Vy] - cp8->registers[Vx];
}

void sub_8xyE(chip8 *cp8) { //SHR Vx
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    cp8 ->  registers[0xF] = (cp8 -> registers[Vx] & 0x80u) >> 7u;

    cp8->registers[Vx] <<= 1;
}

void sub_9xy0(chip8 *cp8) { //SNE vx, vy - Pule a próxima instrução se Vx!= Vy
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;

    if (cp8->registers[Vx] != cp8->registers[Vy]) {
        cp8-> pc += 2;
    }
}

void sub_Annn(chip8 *cp8) { //Inicialize index com o endereço
    uint16_t address = cp8->opcode & 0x0FFFu;
    cp8->index = address;
}

void sub_Bnnn(chip8 *cp8) { //JP V0, addr - Pule para nnn + V0
    uint16_t address = cp8->opcode & 0x0FFFu;
    cp8->pc = cp8->registers[0] + address;
}

void sub_Cxkk(chip8 *cp8) { //Ininicialize Vx com número aleatório
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    uint8_t byte = cp8->opcode & 0x00FFu;

    cp8->registers[Vx] = generateRandomNum() & byte; //pode dar problema - checar

    printf("RANDOM NUMBER: %x\n", cp8->registers[Vx]);
}

void sub_Dxyn (chip8 *cp8) { //DRW Vx, Vy, nibble - Draw, desenhe um sprite de n bytes na coordenada (Vx, Vy)
    uint8_t Vx = (cp8-> opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (cp8->opcode & 0x00F0u) >> 4u;

    uint8_t height = cp8->opcode & 0x000Fu;

    uint8_t xCoord = cp8->registers[Vx] % SCREEN_WIDTH;
    uint8_t yCoord = cp8->registers[Vy] % SCREEN_HEIGHT;

    cp8->registers[0xF] = 0;

    for (unsigned int aux = 0; aux < height; aux++) {
        uint8_t sprite = cp8->memory[(cp8->index) + aux];

        for (unsigned int collumn = 0; collumn < 8; collumn++) {
            uint8_t spritePixel = sprite & (0x80u >> collumn);
            uint32_t * screenPixel = &(cp8->video[(yCoord + aux) * SCREEN_WIDTH + (xCoord + collumn)]);
            
            if (spritePixel) {
                if (*screenPixel == 0xFFFFFFFF) {
                    cp8->registers[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF; //Ou excludente XOR
            } //Se verdadeiro
        }

    }
}

void sub_Ex9E(chip8 * cp8) { //pule para a instrução seguinte se o valor Vx for pressionado
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    uint8_t key = cp8->registers[Vx];

    if (cp8->keypad[key]) {
        cp8->pc += 2;
    }
}

void sub_ExA1(chip8 * cp8) { //pule para a instrução seguinte se o valor Vx NÃO for pressionado
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    uint8_t key = cp8->registers[Vx];

    if (!(cp8->keypad[key])) {
        cp8->pc += 2;
    }
}

void sub_Fx07 (chip8 * cp8) {// LD Vx, DT - Vx = Delay timer
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
    cp8->registers[Vx] = cp8->delayTimer;
}

void sub_Fx0A  (chip8 * cp8) { //Esperar entrada do teclado, guardar valor no registro
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    if (cp8->keypad[0])
	{
		cp8->registers[Vx] = 0;
	}
	else if (cp8->keypad[1])
	{
		cp8->registers[Vx] = 1;
	}
	else if (cp8->keypad[2])
	{
		cp8->registers[Vx] = 2;
	}
	else if (cp8->keypad[3])
	{
		cp8->registers[Vx] = 3;
	}
	else if (cp8->keypad[4])
	{
		cp8->registers[Vx] = 4;
	}
	else if (cp8->keypad[5])
	{
		cp8->registers[Vx] = 5;
	}
	else if (cp8->keypad[6])
	{
		cp8->registers[Vx] = 6;
	}
	else if (cp8->keypad[7])
	{
		cp8->registers[Vx] = 7;
	}
	else if (cp8->keypad[8])
	{
		cp8->registers[Vx] = 8;
	}
	else if (cp8->keypad[9])
	{
		cp8->registers[Vx] = 9;
	}
	else if (cp8->keypad[10])
	{
		cp8->registers[Vx] = 10;
	}
	else if (cp8->keypad[11])
	{
		cp8->registers[Vx] = 11;
	}
	else if (cp8->keypad[12])
	{
		cp8->registers[Vx] = 12;
	}
	else if (cp8->keypad[13])
	{
		cp8->registers[Vx] = 13;
	}
	else if (cp8->keypad[14])
	{
		cp8->registers[Vx] = 14;
	}
	else if (cp8->keypad[15])
	{
		cp8->registers[Vx] = 15;
	}
	else
	{
		cp8->pc -= 2;
	}

}

void sub_Fx15(chip8 * cp8) { //delay timer
    uint16_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    cp8->delayTimer = cp8->registers[Vx];

}

void sub_Fx18(chip8 * cp8) { //Sound timer
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    cp8->soundTimer = cp8->registers[Vx];
}

void sub_Fx1E(chip8 *cp8) { //ADD, I, Vx
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;

    cp8->index += cp8->registers[Vx];
}

void sub_Fx29(chip8 *cp8) { //LD F, VX
    uint8_t Vx = (cp8->opcode & 0x0F00u) >>8u;
    uint8_t digit = cp8->registers[Vx];

    cp8->index = FONTARR_START_ADDRESS + (5 * digit);
} 

void sub_Fx33(chip8 *cp8) { //GUARDE AS REPRESENTAÇÕES BCD de VX nos índices I~I+3
    uint8_t Vx = (cp8->opcode & 0x0F00u) >> 8u;
	uint8_t value = cp8->registers[Vx];

	cp8->memory[cp8->index + 2] = value % 10;
	value /= 10;

	cp8->memory[cp8->index + 1] = value % 10;
	value /= 10;

	cp8->memory[cp8->index] = value % 10;
}

void sub_Fx55(chip8 *cp8) {
    uint8_t Vx = (cp8->opcode & 0x0F00u) >>8u;

	for (uint8_t i = 0; i <= Vx; i++)
	{
		cp8->memory[(cp8->index) + i] = cp8->registers[i];
	}

}

void sub_Fx65(chip8 *cp8) {
    uint8_t Vx = (cp8->opcode & 0x0F00u) >>8u;

	for (uint8_t i = 0; i <= Vx; i++)
	{
		cp8->registers[i] = cp8->memory[cp8->index + i];
	}

}



void choose_sub_0(chip8 * cp8) {
    uint16_t opcode = (cp8->opcode & 0x000Fu);
    switch (opcode) {
        case 0x0:
            sub_00E0(cp8);
            break;
        case 0xE:
            sub_00EE(cp8);    
            break;
        default:
            sub_default(cp8);
            break;
    }
}

void choose_sub_8(chip8 * cp8) {
    uint16_t opcode = (cp8->opcode &  0x000Fu);
    printf("Entering sub_8, opcode = %x\n", opcode);
    switch (opcode) {
        case 0x0:
            sub_8xy0(cp8);
            break;
        case 0x1:
            sub_8xy1(cp8);
            break;
        case 0x2:
            sub_8xy2(cp8);
            break;
        case 0x3:
            sub_8xy3(cp8);
            break;
        case 0x4:
            sub_8xy4(cp8);
            break;
        case 0x5:
            sub_8xy5(cp8);
            break;
        case 0x6:
            sub_8xy6(cp8);
            break;
        case 0x7:
            sub_8xy7(cp8);
            break;
        case 0xE:
            sub_8xyE(cp8);
            break;
        default:
            sub_default(cp8);
            break;
    }
}

void choose_sub_E(chip8 * cp8) {
    uint16_t opcode = (cp8->opcode & 0x000Fu);
        printf("Entering sub_E, opcode = %x\n", opcode);
    switch (opcode) {
        case 0x1:
            sub_ExA1(cp8);
            break;
        case 0xE:
            sub_Ex9E(cp8);
            break;
        default:
            sub_default(cp8);
            break;
    }
}

void choose_sub_F(chip8 * cp8) {
    uint16_t opcode = (cp8->opcode & 0x00FFu);
        printf("Entering sub_F, opcode = %x\n", opcode);
    switch (opcode) {
        case 0x07:
            sub_Fx07(cp8);
            break;
        case 0x0A:
            sub_Fx0A(cp8);
            break;
        case 0x15:
            sub_Fx15(cp8);
            break;
        case 0x18:
            sub_Fx18(cp8);
            break;
        case 0x1E:
            sub_Fx1E(cp8);
            break;
        case 0x29:
            sub_Fx29(cp8);
            break;
        case 0x33:
            sub_Fx33(cp8);
            break;
        case 0x55:
            sub_Fx55(cp8);
            break;
        case 0x65:
            sub_Fx65(cp8);
            break;
        default:
            sub_default(cp8);
            break;
    }
}

void executeOp(chip8 * cp8) {
    uint16_t opcode = (cp8->opcode & 0xF000u) >>12u;

    printf("Executing... opcode = %x\n ", opcode);
    switch (opcode)
    {
    case 0x0:
        choose_sub_0(cp8);
        break;
    case 0x1:
        sub_1nnn(cp8);
        break;
    case 0x2:
        sub_2nnn(cp8);
        break;
    case 0x3:
        sub_3xkk(cp8);
        break;
    case 0x4:
        sub_4xkk(cp8);
        break;
    case 0x5:
        sub_5xy0(cp8);
        break;
    case 0x6:
        sub_6xkk(cp8);
        break;
    case 0x7:
        sub_7xkk(cp8);
        break;
    case 0x8:
        choose_sub_8(cp8);
        break;
    case 0x9:
        sub_9xy0(cp8);
        break;
    case 0xA:
        sub_Annn(cp8);
        break;
    case 0xB:
        sub_Bnnn(cp8);
        break;
    case 0xC:
        sub_Cxkk(cp8);
        break;
    case 0xD:
        sub_Dxyn(cp8);
        break;
    case 0xE:
        choose_sub_E(cp8);
        break;
    case 0xF:
        choose_sub_F(cp8);
    default:
        sub_default(cp8);
        break;
    }

}

void execution_cycle(chip8 *cp8) {

    cp8->opcode = (cp8->memory[cp8->pc] << 8u) | cp8->memory[(cp8->pc)+1];

    cp8->pc +=2;

    executeOp(cp8);


    if(cp8->delayTimer > 0) {
        cp8->delayTimer --;
    }

    if(cp8->soundTimer > 0) {
        cp8->soundTimer--;
    }
}


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


