#include <iostream>
#include "../include/chip8.h"
#include "../include/defines.h"
#include "../include/platform.h"
#include <unistd.h> // for delay
#include <limits>   // For std::numeric_limits
#include <windows.h>

#undef main

std::string games[] = {"invaders", "tetris", "pumpkin", "danm8ku", "rocket2", "ibm", "brix"};

int main(int argc, char *argv[])
{
    // initlizing the chip
    std::cout << "[PENDING] Initializing CHIP-8\n";
    Chip8 chip8;
    std::cout << "[OK] DONE!\n";

    // getting the game
    char *file;
    std::cout << "Pick the a game to execute, input a number [1-6]:\n";
    std::cout << "1- Invaders\n";
    std::cout << "2- Tetris\n";
    std::cout << "3- Pumpkin Dressing up\n";
    std::cout << "4- Danm8ku\n";
    std::cout << "5- Rocket 2\n";
    std::cout << "6- IBM LOGO\n";
    std::cout << "7- Brix\n";
    while (true)
    {
        int x;
        std::cin >> x;
        if (std::cin.fail() || x < 1 || x > 7)
        {
            std::cout << "Invalid input!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        }
        else
        {
            std::string tmp = "../ROMs/" + games[x - 1] + ".ch8";
            file = strdup(tmp.c_str());
            break;
        }
    }

    // loading the rom
    std::cout << "[PENDING] Loading ROM...\n";
    if (!chip8.loadROM(file))
    {
        std::cerr << "[FAILED] Could't Load the ROM\n";
        return 1;
    }
    std::cout << "[OK] ROM Loaded Successfully!\n";

    // setting the SDL window
    std::cout << "[PENDING] Initializing Screen\n";
    Platform platform;
    std::cout << "[OK] Screen Initialized Successfully\n";

    // main loop
    bool quit = false;
    while (!quit)
    {
        bool draw = false;
        bool sound = false;

        chip8.clock(draw, sound);

        quit = platform.inputHandler(chip8.keypad);
        if (quit)
            break;

        if (draw)
            platform.updateScreen(chip8.display);

        if (sound)
            Beep(1400, 160);

        // Delay to emulate CHIP-8's clock speed (adjust as needed)
        usleep(1000);
    }

    return 0;
}