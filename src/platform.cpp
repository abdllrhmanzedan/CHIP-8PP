#include "../include/platform.h"

Platform::Platform()
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("CHIP-8++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              DISPLAY_WIDHT * 8, DISPLAY_HEIGHT * 8, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Platform::updateScreen(u8 *display)
{
    // draw_color = black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

    // clear the current render with the current draw color (black)
    SDL_RenderClear(renderer);

    // draw_color = white
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < DISPLAY_WIDHT; x++)
        {
            // if the pixel is on
            if (display[(y * DISPLAY_WIDHT) + x])
            {
                SDL_Rect rect;

                rect.x = x * 8;
                rect.y = y * 8;
                rect.w = 8;
                rect.h = 8;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    // update the display
    SDL_RenderPresent(renderer);
}

bool Platform::inputHandler(u8 *keypad)
{
    SDL_Event event;

    bool quit = 0;

    if (SDL_PollEvent(&event))
    {
        auto keyboard_state = SDL_GetKeyboardState(nullptr);

        if (event.type == SDL_QUIT)
        {
            quit = 1;
        }
        else
        {
            // update chip8 keypad
            for (int i = 0; i < KEYPAD_SIZE; i++)
            {
                // i will be on if the corresponding button is pressed
                auto corresponding = keypad_to_keyboard[i];
                keypad[i] = keyboard_state[corresponding];
            }
        }
    }
    return quit;
}

Platform::~Platform()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}