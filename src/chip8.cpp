#include "../include/chip8.h"

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstring>

Chip8::Chip8()
{
    // initalize program counter
    pc = PROGRAM_START;

    // intilize the: V, keypad, memory, stack, display
    memset(V      , 0, sizeof(V));
    memset(keypad , 0, sizeof(keypad));
    memset(memory , 0, sizeof(memory));
    memset(stack  , 0, sizeof(stack));
    memset(display, 0, sizeof(display));

    // write fonts into memory
    initFonts();
}

// load ROM into memory starting from PROGRAM_START
bool Chip8::loadROM(char *path)
{
    // open file for reading in binary mode
    std::ifstream program_file(path, std::ios::binary | std::ios::in);
    if (!program_file.is_open())
    {
        std::cerr << "Chip8::loadROM(char*), cannot open file!";
        return false;
    }

    // use then cast as there is no get(uint8_t)
    char instr;
    for (int i = 0; program_file.get(instr); i++)
    {
        if (PROGRAM_START + i == MEMORY_SIZE)
        {
            // no enough space for the program
            return false;
        }
        memory[PROGRAM_START + i] = (u8)instr;
    }
    program_file.close();
    return true;
}

// runs one clock fetch/execute cycle
void Chip8::clock()
{
    // fetch current instruction
    // append two bytes, to get full instruction
    u8 hi = memory[pc];
    u8 lo = memory[pc + 1];
    opcode = (hi << 8u) | (lo);

    // go to next instruction
    pc += 2;

    // decode current instruction
    // 1234: nibbles numbered from left to right
    u8 first_nibble  = (opcode & 0xF000);
    u8 second_nibble = (opcode & 0x0F00);
    u8 third_nibble  = (opcode & 0x00F0);
    u8 fourth_nibble = (opcode & 0x000F);
    u8 last_two      = (opcode & 0x00FF);

    // execute the fetched instruction
    switch (first_nibble)
    {
    case 0x0:
        group_0(fourth_nibble);
        break;
    case 0x1:
        op_1NNN();
        break;
    case 0x2:
        op_2NNN();
        break;
    case 0x3:
        op_3XNN();
        break;
    case 0x4:
        op_4XNN();
        break;
    case 0x5:
        op_5XY0();
        break;
    case 0x6:
        op_6XNN();
        break;
    case 0x7:
        op_7XNN();
        break;
    case 0x8:
        group_8(fourth_nibble);
        break;
    case 0xA:
        op_ANNN();
        break;
    case 0xB:
        op_BNNN();
        break;
    case 0xC:
        op_CXNN();
        break;
    case 0xD:
        op_DXYN();
        break;
    case 0xE:
        group_E(fourth_nibble);
        break;
    case 0xF:
        group_F(last_two);
        break;
    default:
        std::cerr << "Chip8::group_0(uint_8_t) ,No such an instruction!";
        break;
    }

    if (delay_timer)
    {
        delay_timer--;
    }
    if (sound_timer)
    {
        sound_timer--;
    }
}

/**********************************************************************************/

void Chip8::initFonts()
{
    for (int i = 0; i < FONTS_COUNT; i++)
    {
        memory[FONTS_START + i] = font_sprite[i];
    }
}
/**********************************************************************************/

// extract ?(NNN)
u16 Chip8::address()
{
    return (opcode & 0x0FFFu);
}

// extract ??(NN)
u8 Chip8::value()
{
    return (opcode & 0x00FFu);
}

// extract ?(X)??
u8 Chip8::regx()
{
    return (opcode & 0x0F00u);
}

// extract ??(Y)?
u8 Chip8::regy()
{
    return (opcode & 0x00F0u);
}
/**********************************************************************************/

// left most bit
u8 Chip8::msb(u8 reg)
{
    return (reg & 0x8u);
}

// right most bit
u8 Chip8::lsb(u8 reg)
{
    return (reg & 0x1u);
}
/**********************************************************************************/

// clear display
void Chip8::op_00E0()
{
    memset(display, 0, sizeof(display));
}

// uncoditional jump
void Chip8::op_1NNN()
{
    pc = address();
}

// jump to subroutine
void Chip8::op_2NNN()
{
    // push to stack and jump
    stack[sp] = pc;
    sp++;
    pc = address();
}

// return from subroutine
void Chip8::op_00EE()
{
    // pop and return
    sp--;
    pc = stack[sp];
}

// if (Vx == NN) pc+=2
void Chip8::op_3XNN()
{
    u8 x = regx();
    u8 NN = value();

    if (V[x] == NN)
    {
        pc += 2;
    }
}

// if (Vx != NN) pc+=2
void Chip8::op_4XNN()
{
    u8 x = regx();
    u8 NN = value();

    if (V[x] != NN)
    {
        pc += 2;
    }
}

// if (Vx == Vy) pc+=2
void Chip8::op_5XY0()
{
    u8 x = regx();
    u8 y = regy();

    if (V[x] == V[y])
    {
        pc += 2;
    }
}

// if (Vx != Vy) pc+=2
void Chip8::op_9XY0()
{
    u8 x = regx();
    u8 y = regy();

    if (V[x] != V[y])
    {
        pc += 2;
    }
}

// vx = NN
void Chip8::op_6XNN()
{
    u8 x = regx();
    u8 NN = value();

    V[x] = NN;
}

// vx += NN (Vf isn't affected)
void Chip8::op_7XNN()
{
    u8 x = regx();
    u8 NN = value();

    V[x] += NN;
}

// Vx = Vy
void Chip8::op_8XY0()
{
    u8 x = regx();
    u8 y = regy();

    V[x] = V[y];
}

// Vx |= Vy
void Chip8::op_8XY1()
{
    u8 x = regx();
    u8 y = regy();

    V[x] |= V[y];
}

// Vx &= Vy
void Chip8::op_8XY2()
{
    u8 x = regx();
    u8 y = regy();

    V[x] &= V[y];
}

// Vx &= Vy
void Chip8::op_8XY3()
{
    u8 x = regx();
    u8 y = regy();

    V[x] ^= V[y];
}

// Vx += Vy, vf: affected
void Chip8::op_8XY4()
{
    u8 x = regx();
    u8 y = regy();

    V[x] = V[x] + V[y];

    if (V[x] > 255u)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }

    V[x] &= 0xFFu;
}

// Vx -= Vy, vf: affected
void Chip8::op_8XY5()
{
    u8 x = regx();
    u8 y = regy();

    V[x] = V[x] - V[y];

    if (V[x] >= V[y])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
}

// Vx >>= 1, vf: affected
void Chip8::op_8XY6()
{
    u8 x = regx();

    V[0xF] = lsb(V[x]);

    V[x] >>= 1;
}

// Vx = Vy - Vx, Vf: affected
void Chip8::op_8XY7()
{
    u8 x = regx();
    u8 y = regy();

    if (V[y] >= V[x])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }

    V[x] = V[y] - V[x];
}

// Vx <<= 1, vf: affected
void Chip8::op_8XYE()
{
    u8 x = regx();

    V[0xF] = msb(V[x]);

    V[x] <<= 1;
}

// I = NNN
void Chip8::op_ANNN()
{
    index = address();
}

// unconditional jump to (V0 + NNN)
void Chip8::op_BNNN()
{
    pc = V[0] + address();
}

// Vx = rand() & NN
void Chip8::op_CXNN()
{
    u8 x = regx();
    u8 NN = value();

    V[x] = rand() & NN;
}

// draws N bytes read starting from I->I+N
// vf: affected, I: not affected
void Chip8::op_DXYN()
{
    u8 x = regx();
    u8 y = regy();
    u8 n = (opcode & 0x000Fu);

    // V[x], V[y] top left corner
    // V[x]+n-1, V[y]+8-1 bottom right corner
    // cell numbering is reversed here
    // (10, 4) means at 10th column, 4th row
    for (int i = 0; i < n; i++)
    {
        u8 sprite = memory[index + i];
        for (int j = 0; j < 8; j++)
        {
            u8 pixel = sprite & (1 << (7 - i)); // numbering starts from left
            u32 pixel_idx = (V[x] + j) + (V[y] + i) * DISPLAY_WIDHT;

            // All the pixels that are “on” in the sprite will flip the pixels on the screen
            if (pixel)
            {
                // on -> off: flag
                if (display[pixel_idx])
                {
                    V[0xF] = 1;
                }
                display[pixel_idx] ^= 0xFFFFFFFF;
            }
        }
    }
}

// if (key() == Vx) pc+=2
void Chip8::op_EX9E()
{
    u8 x = regx();

    if (keypad[V[x]] != 0)
    {
        pc += 2;
    }
}

// if (key() != Vx) pc+=2
void Chip8::op_EXA1()
{
    u8 x = regx();

    if (keypad[V[x]] == 0)
    {
        pc += 2;
    }
}

// Vx = get_key()
void Chip8::op_FX0A()
{
    u8 x = regx();
    bool pressed = 0;
    for (int i = 0; i < 16; i++)
    {
        if (keypad[i])
        {
            pressed = 1;
            V[x] = i;
            break;
        }
    }

    // stay in same instruction till get input
    if (!pressed)
    {
        pc -= 2;
    }
}

// Vx = delay_timer
void Chip8::op_FX07()
{
    u8 x = regx();

    V[x] = delay_timer;
}

// delay_timer = Vx
void Chip8::op_FX15()
{
    u8 x = regx();

    delay_timer = V[x];
}

// sound_timer = Vx
void Chip8::op_FX18()
{
    u8 x = regx();

    sound_timer = V[x];
}

// I = I + Vx         :Vf=1 if there is overflow
void Chip8::op_FX1E()
{
    u8 x = regx();

    index = index + V[x];

    if (index >= MEMORY_SIZE)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
}

// I = sprite_addr[Vx], reads font
void Chip8::op_FX29()
{
    u8 x = regx();

    index = FONTS_START + 5 * V[x];
}

// if vx = 159: mem[i+0] = 1, mem[i+1] = 5, mem[i+2] = 9
// BCD, Binary coded decimal
void Chip8::op_FX33()
{
    u8 x = regx();
    u8 num = V[x];

    memory[index + 2] = num % 10;
    num /= 10;

    memory[index + 1] = num % 10;
    num /= 10;

    memory[index + 0] = num % 10;
    num /= 10;
}

// mem[i]=v0, mem[i+1]=v1...mem[i+x]=vx. I: doesn't change
void Chip8::op_FX55()
{
    u8 x = regx();

    for (int i = 0; i < x; i++)
    {
        memory[index + i] = V[i];
    }
}

void Chip8::op_FX65()
{
    u8 x = regx();
    u16 temp = index;

    for (int i = 0; i < x; i++)
    {
        V[i] = memory[index + i];
    }
}

void Chip8::op_NULL()
{
    std::cerr << "No such a instruction!";
}

void Chip8::group_0(u8 fourth_nibble)
{
    switch (fourth_nibble)
    {
    case 0x0:
        op_00E0();
        break;
    case 0xE:
        op_00EE();
        break;
    default:
        std::cerr << "Chip8::group_0(uint_8_t) ,No such an instruction!";
        break;
    }
}

void Chip8::group_8(u8 fourth_nibble)
{
    switch (fourth_nibble)
    {
    case 0x0:
        op_8XY0();
        break;
    case 0x1:
        op_8XY1();
        break;
    case 0x2:
        op_8XY2();
        break;
    case 0x3:
        op_8XY3();
        break;
    case 0x4:
        op_8XY4();
        break;
    case 0x5:
        op_8XY5();
        break;
    case 0x6:
        op_8XY6();
        break;
    case 0x7:
        op_8XY7();
        break;
    case 0xE:
        op_8XYE();
        break;
    default:
        std::cerr << "Chip8::group_0(uint_8_t) ,No such an instruction!";
        break;
    }
}

void Chip8::group_E(u8 fourth_nibble)
{
    switch (fourth_nibble)
    {
    case 0xE:
        op_EX9E();
        break;
    case 0x1:
        op_EXA1();
        break;
    default:
        std::cerr << "Chip8::group_0(uint_8_t) ,No such an instruction!";
        break;
    }
}

void Chip8::group_F(u8 last_two)
{
    switch (last_two)
    {
    case 0x07:
        op_FX07();
        break;
    case 0x0A:
        op_FX0A();
        break;
    case 0x15:
        op_FX15();
        break;
    case 0x18:
        op_FX18();
        break;
    case 0x1E:
        op_FX1E();
        break;
    case 0x29:
        op_FX29();
        break;
    case 0x33:
        op_FX33();
        break;
    case 0x55:
        op_FX55();
        break;
    case 0x65:
        op_FX65();
        break;
    default:
        std::cerr << "Chip8::group_0(uint_8_t) ,No such an instruction!";
        break;
    }
}