# Chip-8-Emulator

Emulator for the chip-8 that will load a tetris rom by default if no rom is specified in the args. This was made by pure curiosity and it forced me to be intimate with low level programming. It's a very simple machine, so it's a fun learning experience!

# Dependencies:

You will need the lSDL2 interface library for C/C++ installed.

# Compilation:

Run the following command:
gcc main.c -o <program> -lSDL2

# Running the program:

Run the following command on your terminal:
./<program>

OR


./<program> ./address/to/ch8rom

# Materials that made this project possible:

Building and understanding a Virtual Machine in 2hrs (CHIP-8 Emulator) : https://www.youtube.com/watch?v=jWpbHC6DtnU

Chip 8 technical reference, very useful for the instruction algorithms: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

Emulator 101's guide to chip 8 emulation (C): http://www.emulator101.com/chip-8-sprites.html

Austin Morlan's guide to chip 8 emulation (C++, very helpful for some hard to get instructions): https://austinmorlan.com/posts/chip8_emulator/
