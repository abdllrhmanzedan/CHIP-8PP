#ifndef _CHIP8_H
#define _CHIP8_H

#include "defines.h"

class Chip8
{
public:
    Chip8();
    bool loadROM(char*);                        // load program instruction into the memory
    void clock();                               // perform one clock cycle
    u8 keypad[KEYPAD_SIZE];                     // hexa keypad from [0:F]
    u32 display[DISPLAY_WIDHT * DISPLAY_HEIGHT];// the 64 * 32 screen; u32 to make it easy with SDL
private:
    // data members
    u16 pc;                                     // program counter
    u16 index;                                  // index register
    u16 opcode;                                 // current instruction's opcode
    u8 sp;                                      // stack pointer, points to current place can push in
    u8 delay_timer;                             //
    u8 sound_timer;                             //
    u8 V[16];                                   // V registers from [0:F]
    u8 memory[MEMORY_SIZE];                     // 4KB
    u16 stack[STACK_SIZE];                      // 16 2-bytes-entrie

    // member functions

    void initFonts();                           // save fonts into the memory starting from 0x50:0x9F

    // instruction decoding
    u16 address();                              // gets address for opcodes on form ?NNN
    u8 value();                                 // gets 8-bit value for opcodes of form ??NN
    u8 regx();                                  // gets x for opcodes on form ?x??
    u8 regy();                                  // gets y for opcodes on form ??y?

    // register manipluating
    u8 msb(u8);                                 // most significant bit
    u8 lsb(u8);                                 // least significant bit 

    // opcodes/ Instructions
    void op_00E0();                             // clear display

    void op_1NNN();                             // uncoditional jump to NNN

    void op_2NNN();                             // jump to subrotine *(0xNNN)()
    void op_00EE();                             // return;

    void op_3XNN();                             // if (Vx == NN) pc+=2
    void op_4XNN();                             // if (Vx != NN) pc+=2
    void op_5XY0();                             // if (Vx == Vy) pc+=2
    void op_9XY0();                             // if (Vx != Vy) pc+=2

    void op_6XNN();                             // Vx = NNN
    void op_7XNN();                             // Vx += NN

    void op_8XY0();                             // Vx = Vy
    void op_8XY1();                             // Vx = Vx | Vy
    void op_8XY2();                             // Vx = Vx & Vy
    void op_8XY3();                             // Vx = Vx ^ Vy
    void op_8XY4();                             // Vx = Vx + Vy       :Vf=1 if there is overflow
    void op_8XY5();                             // Vx = Vx - Vy       :Vf=0 if there is underflow
    void op_8XY6();                             // Vx = Vx >> 1       :Vf=LSB before the shift
    void op_8XY7();                             // Vx = Vy - Vx       :Vf=0 if there is underflow
    void op_8XYE();                             // Vx = Vx << 1       :Vf=MSB before the shift

    void op_ANNN();                             // I = NNN
    void op_BNNN();                             // uncoditional jump to (V0 + NNN)
    void op_CXNN();                             // Vx = rand() & NN
    void op_DXYN();                             // bytes to draw are stored starting from I, 8xN rectangle

    void op_EX9E();                             // if (key() == Vx) pc+=2
    void op_EXA1();                             // if (key() != Vx) pc+=2
    void op_FX0A();                             // Vx = get_key(), wait untill event happens

    void op_FX07();                             // Vx = delay_timer
    void op_FX15();                             // delay_timer = Vx
    void op_FX18();                             // sound_timer = Vx

    void op_FX1E();                             // I = I + Vx         :Vf=1 if there is overflow

    void op_FX29();                             // I = sprite_addr[Vx]

    void op_FX33();                             // if vx = 159: mem[i+0] = 1, mem[i+1] = 5, mem[i+2] = 9

    void op_FX55();                             // mem[i]=v0, mem[i+1]=v1...mem[i+x]=vx. I: doesn't change
    void op_FX65();                             // v0=mem[i], v1=mem[i+1]...vx=mem[i+x]. I: doesn't change

    void op_NULL();                             // handling non-valid instruction

    // instruction groups
    void group_0(u8);                           // handling all instructions begin with 0
    void group_8(u8);                           // handling all instructions begin with 8
    void group_E(u8);                           // handling all instructions begin with E
    void group_F(u8);                           // handling all instructions begin with F
};

#endif