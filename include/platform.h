#ifndef _PLATFORM_H
#define _PLATFORM_H

#include "defines.h"
#include "../3rdparty/inc/SDL.h"

class Platform
{
public:
    Platform();
    bool inputHandler(u8 *);                 // handling keypad status, takes Chip8 keypad as an parmater
    void updateScreen(u8 *);
    ~Platform();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};

#endif