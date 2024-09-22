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
    memset(V, 0, sizeof(V));
    memset(keypad, 0, sizeof(keypad));
    memset(memory, 0, sizeof(memory));
    memset(stack, 0, sizeof(stack));
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
void Chip8::clock(bool &draw, bool &sound)
{
    // fetch current instruction
    // append two bytes, to get full instruction
    u8 hi = memory[pc];
    u8 lo = memory[pc + 1];
    opcode = (u16)(hi << 8u) | (lo);

    // go to next instruction
    pc += 2;

    // decode current instruction
    // 1234: nibbles numbered from left to right
    u8 first_nibble = (opcode & 0xF000u) >> 12u;
    u8 second_nibble = (opcode & 0x0F00u) >> 8u;
    u8 third_nibble = (opcode & 0x00F0u) >> 4u;
    u8 fourth_nibble = (opcode & 0x000Fu);
    u8 last_two = (opcode & 0x00FFu);

    // storing the executed code for debugging
    std::string executed;

    // execute the fetched instruction
    switch (first_nibble)
    {
    case 0x0:
        group_0(fourth_nibble, executed);
        break;
    case 0x1:
        executed = "1NNN";
        op_1NNN();
        break;
    case 0x2:
        executed = "2NNN";
        op_2NNN();
        break;
    case 0x3:
        executed = "3XNN";
        op_3XNN();
        break;
    case 0x4:
        executed = "4XNN";
        op_4XNN();
        break;
    case 0x5:
        executed = "5XY0";
        op_5XY0();
        break;
    case 0x6:
        executed = "6XNN";
        op_6XNN();
        break;
    case 0x7:
        executed = "7XNN";
        op_7XNN();
        break;
    case 0x8:
        group_8(fourth_nibble, executed);
        break;
    case 0x9:
        executed = "9XY0";
        op_9XY0();
        break;
    case 0xA:
        executed = "ANNN";
        op_ANNN();
        break;
    case 0xB:
        executed = "BNNN";
        op_BNNN();
        break;
    case 0xC:
        executed = "CXNN";
        op_CXNN();
        break;
    case 0xD:
        draw = true;
        executed = "DXYN";
        op_DXYN();
        break;
    case 0xE:
        group_E(fourth_nibble, executed);
        break;
    case 0xF:
        group_F(last_two, executed);
        break;
    default:
        executed = NO_OPCODE;
        break;
    }

    // debugging
    if (executed == NO_OPCODE)
        debug_print("[FAILED] Unknown opcode: 0x%X\n", opcode);
    else
        debug_print("[OK] %s: 0x%X\n", executed.c_str(), opcode);

    // update timers
    if (delay_timer)
    {
        delay_timer--;
    }
    if (sound_timer)
    {
        if (sound_timer == 1)
            sound = true;
        sound_timer--;
    }
}

void Chip8::initFonts()
{
    for (int i = 0; i < FONTS_COUNT; i++)
    {
        memory[FONTS_START + i] = font_sprite[i];
    }
}
//----------------------------------------------------------------------------------

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
    return (opcode & 0x0F00u) >> 8u;
}

// extract ??(Y)?
u8 Chip8::regy()
{
    return (opcode & 0x00F0u) >> 4u;
}
//----------------------------------------------------------------------------------

// left most bit
u8 Chip8::msb(u8 reg)
{
    return (reg & 0x80u) >> 7u;
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

    // V[0xF] = 0;       // Quirk
    V[x] |= V[y];
}

// Vx &= Vy
void Chip8::op_8XY2()
{
    u8 x = regx();
    u8 y = regy();

    // V[0xF] = 0;       // Quirk
    V[x] &= V[y];
}

// Vx &= Vy
void Chip8::op_8XY3()
{
    u8 x = regx();
    u8 y = regy();

    // V[0xF] = 0;     // Quirk
    V[x] ^= V[y];
}

// Vx += Vy, vf: affected
void Chip8::op_8XY4()
{
    u8 x = regx();
    u8 y = regy();

    u16 sum = (V[x] + V[y]);

    V[x] = (u8)(sum & 0xFFu);

    if (sum > 0xFFu)
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
}

// Vx -= Vy, vf: affected
void Chip8::op_8XY5()
{
    u8 x = regx();
    u8 y = regy();

    u8 old_val = V[x];

    V[x] = V[x] - V[y];

    if (old_val >= V[y])
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
    u8 y = regy();

    V[x] = V[y];
    u8 old_val = V[x];

    V[x] >>= 1;
    V[0xF] = lsb(old_val);
}

// Vx = Vy - Vx, Vf: affected
void Chip8::op_8XY7()
{
    u8 x = regx();
    u8 y = regy();

    u8 old_val = V[y];

    V[x] = V[y] - V[x];

    if (old_val >= V[x])
    {
        V[0xF] = 1;
    }
    else
    {
        V[0xF] = 0;
    }
}

// Vx <<= 1, vf: affected
void Chip8::op_8XYE()
{
    u8 x = regx();
    u8 y = regy();

    V[x] = V[y];
    u8 old_val = V[x];

    V[x] <<= 1;
    V[0xF] = msb(old_val);
}

// I = NNN
void Chip8::op_ANNN()
{
    index = address();
}

// unconditional jump to (V0 + NNN)
void Chip8::op_BNNN()
{
    pc = address() + V[0];
}

// Vx = rand() & NN
void Chip8::op_CXNN()
{
    u8 x = regx();
    u8 NN = value();

    V[x] = (rand() % 256) & NN;
}

// draws N bytes read starting from I->I+N
// vf: affected, I: not affected
void Chip8::op_DXYN()
{
    u8 x = regx();
    u8 y = regy();
    u8 n = (opcode & 0x000Fu);

    V[0xF] = 0;

    // V[x], V[y] top left corner
    // V[x]+n-1, V[y]+8-1 bottom right corner
    // cell numbering is reversed here
    // (10, 4) means at 10th column, 4th row
    for (int i = 0; i < n; i++)
    {
        if ((unsigned int)(index + i) >= MEMORY_SIZE)
        {
            std::cerr << "Chip8::op_DXYN() ,index out of bounds";
            exit(1);
        }

        // clipping
        if (V[y] % DISPLAY_HEIGHT + i == DISPLAY_HEIGHT)
            break;

        u8 sprite = memory[index + i];
        for (int j = 0; j < 8; j++)
        {
            u8 pixel = sprite & (0x80u >> j); // numbering starts from left
            u32 pixel_idx = (V[x] % DISPLAY_WIDHT + j) + (V[y] % DISPLAY_HEIGHT + i) * DISPLAY_WIDHT;

            // clipping
            if (V[x] % DISPLAY_WIDHT + j == DISPLAY_WIDHT)
                break;

            // All the pixels that are “on” in the sprite will flip the pixels on the screen
            if (pixel)
            {
                // on -> off: flag
                if (display[pixel_idx])
                {
                    V[0xF] = 1;
                }
                display[pixel_idx] ^= 0xFFu;
            }
        }
    }
}

// if (key() == Vx) pc+=2
void Chip8::op_EX9E()
{
    u8 x = regx();

    if (keypad[V[x]])
    {
        pc += 2;
    }
}

// if (key() != Vx) pc+=2
void Chip8::op_EXA1()
{
    u8 x = regx();

    if (!keypad[V[x]])
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
            V[x] = (u8)i;
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
    int num = (int)V[x];
    std::cout << num << "\n";
    memory[index + 2] = (u8)(num % 10);
    num /= 10;

    memory[index + 1] = (u8)(num % 10);
    num /= 10;

    memory[index + 0] = (u8)(num % 10);
    std::cout << (int)memory[index] << " " << (int)memory[index + 1] << " " << (int)memory[index + 2] << '\n';
    // exit(1);
}

// mem[i]=v0, mem[i+1]=v1...mem[i+x]=vx. I: doesn't change
void Chip8::op_FX55()
{
    u8 x = regx();

    for (int i = 0; i <= x; i++)
    {
        memory[index + i] = V[i];
    }
    if (QUIRK)
        index += x + 1;
}

void Chip8::op_FX65()
{
    u8 x = regx();

    for (int i = 0; i <= x; i++)
    {
        V[i] = memory[index + i];
    }
    if (QUIRK)
        index += x + 1;
}

void Chip8::group_0(u8 fourth_nibble, std::string &executed)
{
    switch (fourth_nibble)
    {
    case 0x0:
        executed = "00E0";
        op_00E0();
        break;
    case 0xE:
        executed = "00EE";
        op_00EE();
        break;
    default:
        executed = NO_OPCODE;
        break;
    }
}

void Chip8::group_8(u8 fourth_nibble, std::string &executed)
{
    switch (fourth_nibble)
    {
    case 0x0:
        executed = "8XY0";
        op_8XY0();
        break;
    case 0x1:
        executed = "8XY1";
        op_8XY1();
        break;
    case 0x2:
        executed = "8XY2";
        op_8XY2();
        break;
    case 0x3:
        executed = "8XY3";
        op_8XY3();
        break;
    case 0x4:
        executed = "8XY4";
        op_8XY4();
        break;
    case 0x5:
        executed = "8XY5";
        op_8XY5();
        break;
    case 0x6:
        executed = "8XY6";
        op_8XY6();
        break;
    case 0x7:
        executed = "8XY7";
        op_8XY7();
        break;
    case 0xE:
        executed = "8XYE";
        op_8XYE();
        break;
    default:
        executed = NO_OPCODE;
        break;
    }
}

void Chip8::group_E(u8 fourth_nibble, std::string &executed)
{
    switch (fourth_nibble)
    {
    case 0xE:
        executed = "EX9E";
        op_EX9E();
        break;
    case 0x1:
        executed = "EXA1";
        op_EXA1();
        break;
    default:
        executed = NO_OPCODE;
        break;
    }
}

void Chip8::group_F(u8 last_two, std::string &executed)
{
    switch (last_two)
    {
    case 0x07:
        executed = "FX07";
        op_FX07();
        break;
    case 0x0A:
        executed = "FX0A";
        op_FX0A();
        break;
    case 0x15:
        executed = "FX15";
        op_FX15();
        break;
    case 0x18:
        executed = "FX18";
        op_FX18();
        break;
    case 0x1E:
        executed = "FX1E";
        op_FX1E();
        break;
    case 0x29:
        executed = "FX29";
        op_FX29();
        break;
    case 0x33:
        executed = "FX33";
        op_FX33();
        break;
    case 0x55:
        executed = "FX55";
        op_FX55();
        break;
    case 0x65:
        executed = "FX65";
        op_FX65();
        break;
    default:
        executed = NO_OPCODE;
        break;
    }
}