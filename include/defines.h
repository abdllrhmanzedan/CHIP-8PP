#ifndef _DEFINES_H
#define _DEFINES_H

#include <cinttypes>
#include <iostream>
#include "../3rdparty/inc/SDL.h"

// Defines
#define u8             uint8_t
#define i8             int8_t
#define u16            uint16_t
#define u32            uint32_t
#define MEMORY_SIZE    4096
#define STACK_SIZE     16
#define KEYPAD_SIZE    16
#define DISPLAY_WIDHT  64
#define DISPLAY_HEIGHT 32
#define FONTS_COUNT    80              // takes 5-bytes by each charcater 5*16 = 80
#define FONTS_START    0x50            // [0x50:0x9F] 0x50+80=0x9F
#define PROGRAM_START  0x200           // Start of most Chip-8 programs
#define DELAY_TIME     700
#define QUIRK          1

#define DEBUG 1
#define NO_OPCODE "XXXX"
#define debug_print(format, ...)                  \
    do                                         \
    {                                          \
        if (DEBUG)                             \
            fprintf(stderr, format, __VA_ARGS__); \
    } while (0)

// Global Data
inline const u8 font_sprite[FONTS_COUNT] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0,      // 0
    0x20, 0x60, 0x20, 0x20, 0x70,      // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,      // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,      // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,      // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,      // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,      // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,      // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,      // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,      // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,      // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,      // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,      // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,      // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,      // E
    0xF0, 0x80, 0xF0, 0x80, 0x80       // F
};


/*
 * Keypad       Keyboard
 * +-+-+-+-+    +-+-+-+-+
 * |1|2|3|C|    |1|2|3|4|
 * +-+-+-+-+    +-+-+-+-+
 * |4|5|6|D|    |Q|W|E|R|
 * +-+-+-+-+ => +-+-+-+-+
 * |7|8|9|E|    |A|S|D|F|
 * +-+-+-+-+    +-+-+-+-+
 * |A|0|B|F|    |Z|X|C|V|
 * +-+-+-+-+    +-+-+-+-+
*/
inline const SDL_Scancode keypad_to_keyboard[KEYPAD_SIZE] = 
{
    SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_A,
    SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z, SDL_SCANCODE_C,
    SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F, SDL_SCANCODE_V
};


#endif