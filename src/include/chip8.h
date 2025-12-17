

#define START_ADDRESS 0x200 //Instruções de software começam a partir do 0x200 pois 0x000 ~ 0xFFF são reservadas
#define FONTARR_SIZE 80 // 16 chars de 5 bytes
#define FONTARR_START_ADDRESS 0x50 //endereço de onde é esperada a presença do primeiro caractere
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define MAX_GAME_SIZE (0x1000 - 0x200)


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

uint8_t generateRandomNum(void);

chip8 * newChip8(void);
void closeChip8(chip8* cp8);
void loadFile(const char* fileAddress, chip8 * cp8);
void loadBasicFontsIntoMemory(chip8 * cp8);
void execution_cycle(chip8 * cp8);
void executeOp(chip8 * cp8);
void sub_00E0(chip8 * cp8);
void sub_00EE(chip8 * cp8);
void sub_1nnn(chip8 * cp8);
void sub_2nnn(chip8 * cp8);
void sub_3xkk(chip8 * cp8);
void sub_4xkk(chip8 * cp8);
void sub_5xy0(chip8 * cp8);
void sub_6xkk(chip8 * cp8);
void sub_7xkk(chip8 * cp8);
void sub_8xy0(chip8 * cp8);
void sub_8xy1(chip8 * cp8);
void sub_8xy2(chip8 * cp8);
void sub_8xy3(chip8 * cp8);
void sub_8xy4(chip8 * cp8);
void sub_8xy5(chip8 * cp8);
void sub_8xy6(chip8 * cp8);
void sub_8xy7(chip8 * cp8);
void sub_8xyE(chip8 * cp8);
void sub_9xy0(chip8 * cp8);
void sub_Annn(chip8 * cp8);
void sub_Bnnn(chip8 * cp8);
void sub_Cxkk(chip8 * cp8);
void sub_Dxyn(chip8 * cp8);
void sub_Ex9E(chip8 * cp8);
void sub_ExA1(chip8 * cp8);
void sub_Fx07(chip8 * cp8);
void sub_Fx0A(chip8 * cp8);
void sub_Fx15(chip8 * cp8);
void sub_Fx18(chip8 * cp8);
void sub_Fx1E(chip8 * cp8);
void sub_Fx29(chip8 * cp8);
void sub_Fx33(chip8 * cp8);
void sub_Fx55(chip8 * cp8);
void sub_Fx65(chip8 * cp8);
void choose_sub_0(chip8 * cp8);
void choose_sub_8(chip8 * cp8);
void choose_sub_E(chip8 * cp8);
void choose_sub_F(chip8 * cp8);