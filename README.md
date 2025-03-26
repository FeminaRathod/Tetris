# Tetris Console Game

Welcome to **Tetris Console Game**, a classic block-stacking puzzle game built in C++! This version runs directly in your terminal and features different difficulty levels, real-time keyboard controls, and even sound effects to enhance the experience.

## 🎮 Features
- **7 Classic Tetrominoes**: The game includes all the familiar Tetris shapes.
- **Keyboard Controls**: Move, rotate, and drop pieces using simple key inputs.
- **Three Difficulty Levels**: Choose between Easy, Medium, and Hard.
- **Obstacle Generation**: Hard mode adds obstacles for an extra challenge.
- **Scoring System**: Earn points for clearing lines and try to beat your high score!
- **Sound Effects**: Enjoy sound feedback when placing pieces, clearing lines, and hitting game over.
- **High Score Tracking**: The game saves your highest score in a file.

## 🕹️ Controls
- **A** → Move left
- **D** → Move right
- **S** → Move down
- **W** → Rotate piece
- **Spacebar** → Hard drop

## 🚀 How to Play
1. Clone this repository:
   ```sh
   git clone https://github.com/your-username/tetris-console-game.git
   cd tetris-console-game
   ```
2. Compile the game using g++:
   ```sh
   g++ tetris.cpp -o tetris -lwinmm
   ```
3. Run the game:
   ```sh
   ./tetris
   ```
4. Select a difficulty level and start stacking blocks!

## 🏆 High Score
The game keeps track of your best score and stores it in `highscore.txt`. Try to beat your own record!

## ⚙️ Requirements
- **Windows OS** (for `windows.h` and `conio.h` support)
- **C++ Compiler** (MinGW recommended for Windows)

## 🛠️ Possible Enhancements
- Adding multiplayer mode
- Implementing a GUI version
- More game modes (e.g., Marathon, Timed Mode)

## 🎉 Have Fun!
Enjoy playing Tetris in your terminal, and don't forget to challenge yourself to higher scores!

