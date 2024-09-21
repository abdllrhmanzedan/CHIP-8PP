#include <iostream>
#include "../include/chip8.h"
#include "../include/defines.h"
#include "../include/platform.h"
#include <unistd.h>

#undef main

int main(int argc, char *argv[])
{
    std::cout << "[STARTING] TESTING CHIP-8";
    // while (true)
    // {
    //     std::cout << "Pick the a test to execute:\n";
    //     std::cout << "1 - CHIP-8 splash screen\n";
    //     std::cout << "2 - IBM logo\n";
    //     std::cout << "3 - Corax+ opcode test\n";
    //     std::cout << "4 - Flags test\n";
    // }

    std::cout << "[PENDING] Initializing CHIP-8\n";
    Chip8 chip8;
    std::cout << "[OK] DONE!\n";

    std::cout << "[PENDING] Loading ROM...\n";
    char *x = strdup("../tests/test4.ch8");
    bool loaded = chip8.loadROM(x);
    if (!loaded)
    {
        std::cerr << "[FAILED] Could't Load the ROM\n";
        return 1;
    }
    std::cout << "[OK] ROM Loaded Successfully!\n";

    std::cout << "[PENDING] Initializing Display\n";
    Platform platform;
    std::cout << "[OK] Display Initialized Successfully\n";

    int cnt = 0;
    while (true)
    {
        bool draw = false;
        bool quit = false;

        chip8.clock(draw);
        quit = platform.inputHandler(chip8.keypad);
        if (quit)
        {
            break;
        }

        if (draw)
        {
            platform.updateScreen(chip8.display);
        }
        // delay to emulate chip-8's clock speed.
        usleep(15000);
    }
    return 0;
}