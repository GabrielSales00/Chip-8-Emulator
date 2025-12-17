# CHIP-8 Emulator

A simple **CHIP-8 emulator written in C**, created out of pure curiosity and as a hands-on way to explore low-level programming and virtual machines.

CHIP-8 itself is a simple virtual machine, which requires specifications and knowledge very close to those of real hardware, making it an excellent starting point for learning how emulators work. This project implements the CHIP-8 instruction set and provides basic graphics and input using the **SDL2**.

---

## Features

- Complete CHIP-8 virtual machine implementation
- Supports most standard CHIP-8 instructions
- Uses **SDL2** for graphics, input, and frequency handling
- Loads ROMs through the argument

---

## Dependencies

You will need the **SDL2 development library** installed:

### Linux (Debian/Ubuntu)
```bash
sudo apt install libsdl2-2.0-0
```

---

## Building

At the local folder, run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

## Executing

Add executing permissions to the binary:

```bash
chmod +x ./chip8_emu
```


Run the emulator as follows:

```bash
./chip8_emu <path/to/rom>
```
---

## Materials that made this project possible:

Building and understanding a Virtual Machine in 2hrs (CHIP-8 Emulator) : https://www.youtube.com/watch?v=jWpbHC6DtnU

Chip 8 technical reference, very useful for the instruction algorithms: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

Emulator 101's guide to chip 8 emulation (C): http://www.emulator101.com/chip-8-sprites.html

Austin Morlan's guide to chip 8 emulation (C++, very helpful for some hard to get instructions): https://austinmorlan.com/posts/chip8_emulator/
