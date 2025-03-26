#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>

using namespace std;

const int WIDTH = 50;
const int HEIGHT = 20;
const char BLOCK = '#';

const vector<vector<vector<int>>> tetrominoes = {
    {{1, 1, 1, 1}},
    {{1, 1}, {1, 1}},
    {{0, 1, 0}, {1, 1, 1}},
    {{0, 1, 1}, {1, 1, 0}},
    {{1, 1, 0}, {0, 1, 1}},
    {{1, 0, 0}, {1, 1, 1}},
    {{0, 0, 1}, {1, 1, 1}}
};

void playDropSound() { Beep(800, 100); }
void playLineClearSound() { Beep(1000, 150); }
void playGameOverSound() { Beep(500, 300); }

struct Position {
    int x, y;
};

class Tetromino {
public:
    vector<vector<int>> shape;
    Position pos;

    Tetromino(int type) {
        shape = tetrominoes[type];
        pos = {WIDTH / 2 - 1, 0};
    }

    void rotate() {
        int oldRows = shape.size();
        int oldCols = shape[0].size();
        vector<vector<int>> rotated(oldCols, vector<int>(oldRows, 0));
        for (int i = 0; i < oldRows; i++)
            for (int j = 0; j < oldCols; j++)
                rotated[j][oldRows - i - 1] = shape[i][j];
        shape = rotated;
    }
};

class TetrisGame {
private:
    vector<vector<int>> grid;
    Tetromino* currentPiece;
    bool gameOver;
    int score, level, speed, highScore, difficulty;
    HANDLE hConsole;

public:
    TetrisGame(int diff) : difficulty(diff) {
        grid = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, 0));
        currentPiece = new Tetromino(rand() % 7);
        gameOver = false;
        score = 0;
        level = 1;
        speed = (difficulty == 2) ? 400 : (difficulty == 3) ? 300 : 500;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        highScore = loadHighScore();
        if (difficulty == 3) generateObstacles();
    }

    int loadHighScore() {
        ifstream file("highscore.txt");
        int hs = 0;
        if (file) file >> hs;
        file.close();
        return hs;
    }

    void saveHighScore() {
        if (score > highScore) {
            highScore = score;
            ofstream file("highscore.txt");
            file << highScore;
            file.close();
        }
    }

    void generateObstacles() {
        int obstacleRows = HEIGHT / 4;
        for (int i = HEIGHT - obstacleRows; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (rand() % 3 == 0) grid[i][j] = 1;
            }
        }
    }

    bool isValidMove(Tetromino* piece, int newX, int newY) {
        for (int i = 0; i < piece->shape.size(); i++) {
            for (int j = 0; j < piece->shape[i].size(); j++) {
                if (piece->shape[i][j]) {
                    int x = newX + j, y = newY + i;
                    if (x < 0 || x >= WIDTH || y >= HEIGHT || (y >= 0 && grid[y][x])) return false;
                }
            }
        }
        return true;
    }

    void clearFullLines() {
        for (int y = HEIGHT - 1; y >= 0; y--) {
            bool isFull = true;
            for (int x = 0; x < WIDTH; x++) {
                if (grid[y][x] == 0) {
                    isFull = false;
                    break;
                }
            }
            if (isFull) {
                playLineClearSound();
                score += 10;
                for (int row = y; row > 0; row--) {
                    grid[row] = grid[row - 1];
                }
                grid[0] = vector<int>(WIDTH, 0);
                y++;  // Recheck the same line after shifting
            }
        }
    }

    void placePiece() {
        for (int i = 0; i < currentPiece->shape.size(); i++) {
            for (int j = 0; j < currentPiece->shape[i].size(); j++) {
                if (currentPiece->shape[i][j]) {
                    int x = currentPiece->pos.x + j, y = currentPiece->pos.y + i;
                    if (y >= 0) grid[y][x] = 1;
                }
            }
        }
        playDropSound();
        clearFullLines();
        delete currentPiece;
        currentPiece = new Tetromino(rand() % 7);
        if (!isValidMove(currentPiece, currentPiece->pos.x, currentPiece->pos.y)) {
            gameOver = true;
            playGameOverSound();
            saveHighScore();
        }
    }

    void handleInput() {
        if (_kbhit()) {
            char input = _getch();
            switch (input) {
                case 'a':  // Move left
                    if (isValidMove(currentPiece, currentPiece->pos.x - 1, currentPiece->pos.y)) 
                        currentPiece->pos.x--;
                    break;
                case 'd':  // Move right
                    if (isValidMove(currentPiece, currentPiece->pos.x + 1, currentPiece->pos.y)) 
                        currentPiece->pos.x++;
                    break;
                case 's':  // Move down
                    if (isValidMove(currentPiece, currentPiece->pos.x, currentPiece->pos.y + 1)) 
                        currentPiece->pos.y++;
                    break;
                case 'w':  // Rotate
                    currentPiece->rotate();
                    if (!isValidMove(currentPiece, currentPiece->pos.x, currentPiece->pos.y)) 
                        currentPiece->rotate();  // Undo rotation if invalid
                    break;
                case ' ':  // Hard drop
                    while (isValidMove(currentPiece, currentPiece->pos.x, currentPiece->pos.y + 1)) 
                        currentPiece->pos.y++;
                    placePiece();
                    break;
            }
        }
    }

    void update() {
        if (isValidMove(currentPiece, currentPiece->pos.x, currentPiece->pos.y + 1)) {
            currentPiece->pos.y++;
        } else {
            placePiece();
        }
    }

    void render() {
        COORD coord = {0, 0};
        SetConsoleCursorPosition(hConsole, coord);
        for (int y = 0; y < HEIGHT; y++) {
            cout << "|";
            for (int x = 0; x < WIDTH; x++) {
                bool isPieceBlock = false;
                for (int i = 0; i < currentPiece->shape.size(); i++) {
                    for (int j = 0; j < currentPiece->shape[i].size(); j++) {
                        int pieceX = currentPiece->pos.x + j;
                        int pieceY = currentPiece->pos.y + i;
                        if (pieceX == x && pieceY == y && currentPiece->shape[i][j] == 1) {
                            cout << BLOCK;
                            isPieceBlock = true;
                            break;
                        }
                    }
                    if (isPieceBlock) break;
                }
                if (!isPieceBlock) cout << (grid[y][x] == 1 ? BLOCK : ' ');
            }
            cout << "|\n";
        }
        cout << "Score: " << score << " Level: " << level << " High Score: " << highScore << endl;
    }

    void run() {
        while (!gameOver) {
            handleInput();
            update();
            render();
            Sleep(speed);
        }
        cout << "Game Over! Final Score: " << score << " High Score: " << highScore << endl;
    }
};
int main() {
    srand(time(0));
    while (true) {
        int difficulty;
        cout << "Select Difficulty (1-Easy, 2-Medium, 3-Hard): ";
        cin >> difficulty;

        TetrisGame game(difficulty);
        game.run();

        char choice;
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;

        // Exit the loop only if the user explicitly chooses not to continue
        if (choice != 'y' && choice != 'Y') {
            cout << "Thanks for playing! Goodbye!" << endl;
            break;
        }
    }
    return 0;
}