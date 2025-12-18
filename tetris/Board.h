#ifndef BOARD_H
#define BOARD_H

#include "TetriminoDefinitions.h"
#include <queue>

namespace Tetris {
class Tetrimino; // Forward declaration
class Board {
public:
    constexpr static int GRID_W = 10;
    constexpr static int GRID_H = 20;
    constexpr static int BLOCK_SIZE = 30; // Pixel size
    constexpr static int BOARD_OFFSET_X = 200;
    constexpr static int BOARD_OFFSET_Y = 50;
    constexpr static int GRAVITY_SPEED = 60; // Frames per drop
    constexpr static int HOLD_PIECE_OFFSET_X = BOARD_OFFSET_X - 100;
    constexpr static int HOLD_PIECE_OFFSET_Y = BOARD_OFFSET_Y;
    constexpr static int NEXT_QUEUE_OFFSET_X = BOARD_OFFSET_X + GRID_W * BLOCK_SIZE + 50;
    constexpr static int NEXT_QUEUE_OFFSET_Y = BOARD_OFFSET_Y;
    constexpr static int NEXT_QUEUE_SPACING = 120;
    constexpr static int NEXT_QUEUE_COUNT = 5;

public:
    Board();
    void init();
    void update(); // Handles gravity for the active piece
    void draw();

    // Game Logic
    bool checkCollision(Tetris::TetriminoType type, int rotation, int x, int y);
    void lockPiece(const Tetrimino& t);

    // Getters for Tetrimino ghost calculations
    bool isOccupied(int x, int y) const;
    void drawHoldPiece(TetriminoType type);

    // Gravity
    void updateGravityTimer(bool rotated, bool moved);

    // Next Queue
    void drawNextQueue(int count);

    // Draw decorations
    void drawDecorations();

private:
    // The main grid: 0 = empty, 1-7 = colors/types
    int grid[20][10];

    // Active piece management
    Tetrimino* activePiece;
    Tetrimino* holdPiece;
    std::queue<Tetrimino*> nextQueue;

    // Gravity
    int gravityTimer;
    int gravitySpeed; // Frames per drop (lower is faster)

    void spawnPiece();
    size_t clearLines();
    bool isPerfectClear();
    void generate7Bag(); // Fills nextQueue
};
}

#endif
