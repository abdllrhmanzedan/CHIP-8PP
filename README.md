<h1 align=center>
CHIP-8++<br>
A CHIP-8 Emulator with C++
</h1>

<div align=center>
<h4> Brix gameplay on CHIP-8++ emulator </h4>

https://github.com/user-attachments/assets/27cee1bd-2e9b-4110-a810-cb6da51cd4db

<a href = "https://www.libsdl.org/">
  <img src="https://img.shields.io/badge/Screen%20Rendering%20Using-SDL-lightblue">
</a>

</div>

## About
**CHIP-8++** is my simple emulator and interpreter for the CHIP-8 virtual machine. I created it for educational purposes to explore the world of emulators and learn more about how they work!

![run](https://github.com/user-attachments/assets/7e381e61-4a69-4139-a270-5107f84b478a)

## Features
- **Memory**: 4KB total, with each cell capable of holding a 8-bit value.
- **Stack**: 16 cells, each holding a 16-bit address used for function/subroutine calls.
- **Display**: Monochrome 64x32 pixel screen.
- **Program Counter (PC)**: A 16-bit register that tracks the next instruction to execute.
- **Timers**: Separate sound and delay timers to manage operations that needs timing.
- **Registers**:
  - **Index Register (I)**: Used for memory access and data management.
  - **16 General Purpose Registers (V0 to VF)**: Hexadecimal numbered, with VF often used for flags.

## Usage
1. Make sure you have a `g++` compiler
2. **Clone the Repository**  
   ```bash
   git clone <repository-url>
   ```
3. **Compile the Code**
   ```bash
   g++ -L 3rdparty/lib src/*.cpp -lSDL2
   ```
4. **Run the Application**
   ```bash
   ./your_executable_name
   ```

<!-- ## Future Improvements
- [ ] Provide GUI with debugger and registers content view!
- [ ] Use function pointers instead of big switch statements.
- [ ] Add SUPER-CHIP support
- [ ] Add XO-CHIP support -->

## Acknowledgments/References
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#add-xo-chip-support)
- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [CHIP-8 Variant Opcode Table](https://chip8.gulrak.net/)
- [CHIP-8 - Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [r/EmuDev](https://www.reddit.com/r/EmuDev/)